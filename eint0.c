

#include <LPC21xx.H>
#include "header.h"
 
int mode;
 
void EINT0_Handler(void) __irq
{
    mode = !mode;     // Toggle between Automatic (0) and Manual (1) mode
    EXTINT = 1;       // Clear EINT0 interrupt flag
    VICVectAddr = 0;  // Acknowledge end of ISR to VIC
}
 
void config_for_eint(void)
{
    PINSEL1     |= 1;                            // Configure P0.16 as EINT0
    EXTMODE     |= 1;                            // Edge-sensitive mode for EINT0
    EXTPOLAR    &= ~0;                           // Falling-edge sensitive
    VICIntSelect = 0;                            // Configure all interrupts as IRQ
    VICVectCntl0 = 14 | (1 << 5);                // Enable Slot 0 for EINT0 (Channel 14)
    VICVectAddr0 = (unsigned int)EINT0_Handler;  // Assign ISR address
    VICIntEnable = (1 << 14);                    // Enable EINT0 interrupt in VIC
}
