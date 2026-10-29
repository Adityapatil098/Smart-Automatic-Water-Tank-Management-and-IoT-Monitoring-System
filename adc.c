#include <LPC21xx.H>
#include "header.h"
 
void adc_init(void)
{
    PINSEL1 = 0x15400000; // Configure P0.27(AIN0), P0.28(AIN1), P0.29(AIN2)
    ADCR    = 0x00200400; // PDN=1 (Operational), CLKDIV=4
}
 
u32 adc_read(u8 ch_num)
{
    int temp;
    ADCR |= (1 << ch_num);          // Select ADC channel
    ADCR |= (1 << 24);              // Start A/D conversion now
    while(((ADDR >> 31) & 1) == 0); // Wait for DONE bit (Bit 31)
    ADCR ^= (1 << 24);              // Stop conversion
    ADCR ^= (1 << ch_num);          // Deselect channel
    temp = ((ADDR >> 6) & 0x3FF);   // Extract 10-bit result (Bits 15:6)
    return temp;
} 
