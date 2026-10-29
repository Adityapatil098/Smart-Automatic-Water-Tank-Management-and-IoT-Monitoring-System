#include <LPC21xx.H>
#include "header.h"
 
#define TRIG (1 << 10) // P0.10 -> Trigger Output
#define ECHO (1 << 11) // P0.11 -> Echo Input
 
void ultrasonic_init(void)
{
    IODIR0 |= TRIG;
    IODIR0 &= ~ECHO;
}
 
u32 ultrasonic_read(void)
{
    u32 count = 0;
    IOCLR0 = TRIG;
    delay_us(2);
    IOSET0 = TRIG;
    delay_us(10);
    IOCLR0 = TRIG;
 
    while(!(IOPIN0 & ECHO)); // Wait for Echo HIGH
    while(IOPIN0 & ECHO)     // Measure Echo HIGH pulse width in us
    {
        count++;
        delay_us(1);
        if(count > 30000)
        {
            uart0_tx_string("timeout\r\n");
            return 0;
        }
    }
    return (count / 58);     // Convert pulse duration to distance in cm
}
