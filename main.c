#include <LPC21xx.H>
#include "header.h"
 
extern int mode;
int t[6];
 
int main()
{
    int adc_value;
    u8 m, h, h1, s1, s, data;
    u8 d, mo, y;
    u32 distance = 62;
    float vout, adcout;
    float water_level, temp;
    unsigned int can_data[2];
 
    IODIR0 |= buz;
    IODIR0 |= RED_LED | YELLOW_LED | GREEN_LED;
    IOSET0  = RED_LED | YELLOW_LED | GREEN_LED;
 
    uart0_init(9600);
    adc_init();
    servo_init();
    lcd_init();
    i2c_init();
    ultrasonic_init();
    config_for_eint();
    //wifi_connect();
    //esp_read();
 
    uart0_tx_string("hello\r\n");
 
    /* Initialize DS1307 RTC Time & Date via I2C0 (Slave Addr: 0xD0) */
    i2c_write(0xD0, 0x02, 0x12); // Hours
    i2c_write(0xD0, 0x01, 0x58); // Minutes
    i2c_write(0xD0, 0x00, 0x55); // Seconds
    i2c_write(0xD0, 0x04, 0x12); // Date
    i2c_write(0xD0, 0x05, 0x08); // Month
    i2c_write(0xD0, 0x06, 0x26); // Year (2026)
 
    delay_ms(1000);
    //servo_180deg();
   
    while(1)
    {
        uart0_tx_string("\r\n========================================\r\n");
        uart0_tx_string("      SMART WATER TANK MANAGEMENT       \r\n");
        uart0_tx_string("========================================\r\n");
        uart0_tx_string("Controller: LPC2129 ARM7\r\n");
        uart0_tx_string("Project Status: RUNNING\r\n");
 
        distance = ultrasonic_read();
 
        adc_value = adc_read(2);
        water_level = (adc_value * 100) / 1023;
        uart0_tx_string("\r\n");
        uart0_integer(water_level);
        delay_sec(1);
 
        adcout = adc_read(0);
        vout = (adcout * 3.3) / 1023;
        temp = vout * 100;
        uart0_tx_string("\r\n");
        uart0_integer(adcout);
        delay_sec(1);
 
        if(mode == 1) // Odd: MANUAL MODE
        {
            uart0_tx_string("\r\nMANUAL MODE\r\n");
            if(water_level < 20)
            {
                uart0_tx_string("\r\nLOW WATER LEVEL\r\n");
                uart0_tx_string("Turn on the motor\r\n");
                IOSET0 = RED_LED | YELLOW_LED;
                IOCLR0 = GREEN_LED;
                while(1)
                {
                    if(sw1 == 0)
                    {
                        while(sw1 == 0);
                        uart0_tx_string("\r\nPUMP STATUS: ON\r\n");
                        //uart0_tx_string("\r\nRELAY STATUS: ON\r\n");
                        uart0_tx_string("\r\nVALVE STATUS: OPEN\r\n");
                        servo_90deg();
                        delay_ms(10);
                        uart0_tx_string("\r\nTemp=");
                        uart0_float(temp);
                        uart0_tx_string("%\r\n");
                        uart0_tx_string("Distance:");
                        uart0_float(distance);
                        uart0_tx_string("\r\n");
                        uart0_tx_string("Water level: ");
                        uart0_integer(water_level);
                        uart0_tx_string("\r\n");
                        delay_ms(50);
                        break;
                    }
                }
            }
            else if(water_level >= 20 && water_level < 50)
            {
                uart0_tx_string("\r\nMEDIUM WATER LEVEL\r\n");
                IOSET0 = GREEN_LED | RED_LED;
                IOCLR0 = YELLOW_LED;
            }
            else if(water_level >= 50)
            {
    IOSET0 = buz;
                delay_ms(100);
                IOCLR0 = buz;
                uart0_tx_string("\r\nHIGH WATER LEVEL\r\n");
                uart0_tx_string("Turn off the motor\r\n");
                IOSET0 = GREEN_LED | YELLOW_LED;
                IOCLR0 = RED_LED;
                while(1)
                {
                    if(sw2 == 0)
                    {
                        while(sw2 == 0);
                        servo_0deg();
                        uart0_tx_string("\r\nPUMP STATUS: OFF\r\n");
                        uart0_tx_string("\r\nRELAY STATUS: OFF\r\n");
                        uart0_tx_string("\r\nVALVE STATUS: CLOSE\r\n");
                        delay_ms(100);
                        uart0_tx_string("\r\nTemp=");
                        uart0_float(temp);
                        uart0_tx_string("%\r\n");
                        uart0_tx_string("Water level: ");
                        uart0_integer(water_level);
                        uart0_tx_string("\r\n");
                        delay_ms(500);
                        uart0_tx_string("Distance:");
                        uart0_float(distance);
                        uart0_tx_string("\r\n");
                        break;
                    }
                }
            }
        }
        else // Even: AUTOMATIC MODE
        {
            uart0_tx_string("\r\nAUTOMATIC MODE\r\n");
            if(water_level < 20)
            {
                uart0_tx_string("\r\nLOW WATER LEVEL\r\n");
                IOSET0 = RED_LED | YELLOW_LED;
                IOCLR0 = GREEN_LED;
                servo_90deg();
                uart0_tx_string("\r\nPUMP STATUS: ON\r\n");
                uart0_tx_string("\r\nRELAY STATUS: ON\r\n");
                uart0_tx_string("\r\nVALVE STATUS: OPEN\r\n");
                uart0_tx_string("\r\nTemp=");
                uart0_float(temp);
                uart0_tx_string("\r\n");
                uart0_tx_string("Distance:");
                uart0_float(distance);
                uart0_tx_string("\r\n");
                uart0_tx_string("Water level: ");
                uart0_integer(water_level);
                uart0_tx_string("\r\n");
                delay_ms(500);
            }
            else if(water_level >= 20 && water_level < 30)
            {
                uart0_tx_string("\r\nMEDIUM WATER LEVEL\r\n");
                IOSET0 = GREEN_LED | RED_LED;
                IOCLR0 = YELLOW_LED;
            }
            else if(water_level >= 30)
            {
                IOSET0 = buz;
                delay_ms(100);
                IOCLR0 = buz;
                uart0_tx_string("\r\nHIGH WATER LEVEL\r\n");
                uart0_tx_string("Turn off the motor\r\n");
                IOSET0 = GREEN_LED | YELLOW_LED;
                IOCLR0 = RED_LED;
                servo_0deg();
                uart0_tx_string("\r\nPUMP STATUS: OFF\r\n");
                uart0_tx_string("\r\nRELAY STATUS: OFF\r\n");
                uart0_tx_string("\r\nVALVE STATUS: CLOSE\r\n");
                uart0_tx_string("\r\nTemp=");
                uart0_float(temp);
                uart0_tx_string("%\r\n");
                uart0_tx_string("Water level: ");
                uart0_integer(water_level);
                uart0_tx_string("\r\n");
                delay_ms(500);
                uart0_tx_string("Distance:");
                uart0_float(distance);
                uart0_tx_string("\r\n");
            }
        }
 
        /* Read Date & Time from DS1307 RTC (0xD1) */
        uart0_tx_string("RTC Date: ");
        d  = i2c_read(0xD1, 0x04);
        mo = i2c_read(0xD1, 0x05);
        y  = i2c_read(0xD1, 0x06);
        uart0_tx(d/16 + 48);
        uart0_tx(d%16 + 48);
        uart0_tx('/');
        uart0_tx(mo/16 + 48);
        uart0_tx(mo%16 + 48);
        uart0_tx('/');
        uart0_tx_string("20");
        uart0_tx(y/16 + 48);
        uart0_tx(y%16 + 48);
        uart0_tx_string("\r\n");
 
        uart0_tx_string("RTC Time: ");
        h = i2c_read(0xD1, 0x02);
        m = i2c_read(0xD1, 0x01);
        s = i2c_read(0xD1, 0x00);
        h1 = h & 0x1F;
        uart0_tx(h1/16 + 48);
        uart0_tx(h1%16 + 48);
        uart0_tx(':');
        uart0_tx(m/16 + 48);
        uart0_tx(m%16 + 48);
        uart0_tx(':');
        uart0_tx(s/16 + 48);
        uart0_tx(s%16 + 48);
        if((h >> 5) & 1)
            uart0_tx_string(" PM\r\n");
        else
            uart0_tx_string(" AM\r\n");
 
        //eeprom(temp);
        //wifi_connect(); // P0.0 -> TX, P0.1 -> RX
 
        can_data[0] = (unsigned int)water_level;
        can_data[1] = (unsigned int)temp;
        can1_parameter((u8)temp, (u8)water_level, (u8)distance);
        delay_sec(5);
    }
}
