#include <LPC21xx.H>
#include "header.h"
 
#define SERVO (1 << 8) // P0.8 -> PWM4
void servo_init(void)
{
    PINSEL0 = (PINSEL0 & ~(3 << 16)) | (2 << 16); // Configure P0.8 as PWM4
    PWMTCR  = (1 << 1);          // Counter Reset
    PWMPR   = 14;                // Prescaler for 1 us tick at 15 MHz PCLK
    PWMMR0  = 20000;             // 20 ms PWM period (50 Hz)
    PWMMR4  = 1500;              // Default 1.5 ms pulse (90 deg)
    PWMMCR  = (1 << 1);          // Reset PWMTC on PWMMR0 match
    PWMLER  = (1 << 0) | (1 << 4); // Enable shadow latch for MR0 & MR4
    PWMPCR  = (1 << 12);         // Enable PWM4 single-edge output
    PWMTCR  = (1 << 0) | (1 << 3); // Enable Counter and PWM Mode
}
 
void servo_0deg(void)
{
    PWMMR4 = 1000;       // 1.0 ms pulse width -> 0 degrees (Valve Closed)
    PWMLER = (1 << 4);
}
 
void servo_90deg(void)
{
    PWMMR4 = 1500;       // 1.5 ms pulse width -> 90 degrees (Valve Open)
    PWMLER = (1 << 4);
}
 
void servo_180deg(void)
{
    PWMMR4 = 2000;       // 2.0 ms pulse width -> 180 degrees
    PWMLER = (1 << 4);
}
