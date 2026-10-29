#include <LPC21xx.H>
#include "header.h"
 
void can1_init(void)
{
    VPBDIV  = 1;          // PCLK = 60 MHz
    PINSEL1 |= 0x00040000; // Configure P0.25 as RD1 (CAN1 RX)
    C1MOD   = 1;          // Select Reset Mode
    C1BTR   = 0x001C001D; // Configure 125 kbps baudrate
    AFMR    = 2;          // Accept all incoming messages (Bypass mode)
    C1MOD   = 0;          // Release Reset Mode (Operating Mode)
}
 
#define TCS ((C1GSR >> 3) & 1)
 
void can1_tx(CAN1 v)
{
    C1TID1 = v.id;
    C1TFI1 = (v.dlc << 16); // Set DLC, RTR=0, FF=0 (Standard 11-bit ID)
    if(v.rtr == 0)          // If Data Frame
    {
        C1TDA1 = v.byteA;
        C1TDB1 = v.byteB;
    }
    else
    {
        C1TFI1 |= (1 << 30); // Remote Transmission Request (RTR=1)
    }
    C1CMR = 0x21;            // Select TxBuf1 & Start Transmission
    while(TCS == 0);         // Wait until transmission is complete
}
 
void can1_parameter(u8 t, u8 w, u8 d)
{
    CAN1 MSG;
    MSG.id    = 0x301;
    MSG.rtr   = 0;
    MSG.dlc   = 8;
    MSG.byteA = 0;
    MSG.byteA |= t;
    MSG.byteA |= (w << 8);
    MSG.byteA |= (d << 16);
    MSG.byteB = 0;
    can1_tx(MSG);
}
