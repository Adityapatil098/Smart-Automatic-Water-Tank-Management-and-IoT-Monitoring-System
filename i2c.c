#include <LPC21xx.H>
#include "header.h"
 
void i2c_init(void)
{
    PINSEL0  |= 0x50;     // P0.2 -> SCL0, P0.3 -> SDA0
    I2CONSET  = (1 << 6); // Enable I2C interface (I2EN=1, Master Mode)
    I2SCLH    = I2SCLL = 75; // 100 kbps standard clock frequency
}
 
#define SI ((I2CONSET >> 3) & 1)
 
void i2c_write(u8 sa, u8 mr, u8 data)
{
    /* 1. Generate START condition */
    I2CONSET = (1 << 5); // STA = 1
    I2CONCLR = (1 << 3); // Clear SI
    while(SI == 0);      // Wait for START condition
    I2CONCLR = (1 << 5); // STA = 0
 
    /* 2. Send Slave Address + Write (SA+W) & check ACK */
    I2DAT    = sa;       // Send SA+W
    I2CONCLR = (1 << 3); // Clear SI
    while(SI == 0);
    if(I2STAT == 0x20)
    {
        uart0_tx_string("\r\nErr: SA+W\r\n");
        goto exit;
    }
 
    /* 3. Send Memory/Register Address & check ACK */
I2DAT    = mr;       // Send memory location
    I2CONCLR = (1 << 3); // Clear SI
    while(SI == 0);
    if(I2STAT == 0x30)
    {
        uart0_tx_string("\r\nErr: Memory Addr\r\n");
        goto exit;
    }
 
    /* 4. Send Data Byte & check ACK */
    I2DAT    = data;     // Send data
    I2CONCLR = (1 << 3); // Clear SI
    while(SI == 0);
    if(I2STAT == 0x30)
    {
        uart0_tx_string("\r\nErr: Data\r\n");
        goto exit;
    }
 
    /* 5. Generate STOP condition */
exit:
    I2CONSET = (1 << 4); // STO = 1
    I2CONCLR = (1 << 3); // Clear SI
}
 
u8 i2c_read(u8 sa, u8 mr)
{
    u8 temp;
 
    /* 1. Generate START condition */
    I2CONSET = (1 << 5); // STA = 1
    I2CONCLR = (1 << 3); // Clear SI
    while(SI == 0);
    I2CONCLR = (1 << 5); // STA = 0
 
    /* 2. Send SA+W & check ACK */
    I2DAT    = sa & 0xFE; // Send SA+W
    I2CONCLR = (1 << 3);  // Clear SI
    while(SI == 0);
    if(I2STAT == 0x20)
    {
        uart0_tx_string("\r\nErr: SA+W\r\n");
        goto exit;
    }
 
    /* 3. Send Memory Address & check ACK */
    I2DAT    = mr;
    I2CONCLR = (1 << 3);
    while(SI == 0);
    if(I2STAT == 0x30)
    {
        uart0_tx_string("\r\nErr: Memory Addr\r\n");
        goto exit;
    }
 
    /* 4. Generate Repeated START condition */
    I2CONSET = (1 << 5); // STA = 1
    I2CONCLR = (1 << 3); // Clear SI
    while(SI == 0);
    I2CONCLR = (1 << 5); // STA = 0
 
    /* 5. Send SA+R & check ACK */
    I2DAT    = sa;       // Send SA+R
    I2CONCLR = (1 << 3);
    while(SI == 0);
    if(I2STAT == 0x48)
    {
        uart0_tx_string("\r\nErr: SA+R\r\n");
        goto exit;
    }
 
    /* 6. Read Data Byte & send NACK */
    I2CONCLR = (1 << 3);
    while(SI == 0);
    temp = I2DAT;
 
    /* 7. Generate STOP condition */
exit:
    I2CONSET = (1 << 4); // STO = 1
    I2CONCLR = (1 << 3); // Clear SI
 
    /* 8. Return received byte */
    return temp;
}
