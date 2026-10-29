#include <LPC21xx.H>
#include "header.h"
 
void lcd_data(unsigned char data)
{
    unsigned int temp;
    IOCLR1 = 0xFE << 16;
    temp   = ((data & 0xF0) << 16);
    IOSET1 = temp;
    IOSET1 = (1 << 17); // RS = 1 (Data Register)
    IOCLR1 = (1 << 18); // RW = 0 (Write)
    IOSET1 = (1 << 19); // EN = 1
    delay_ms(2);
    IOCLR1 = (1 << 19); // EN = 0
 
    IOCLR1 = 0xFE << 16;
    temp   = ((data & 0x0F) << 20);
    IOSET1 = temp;
    IOSET1 = (1 << 17); // RS = 1
    IOCLR1 = (1 << 18); // RW = 0
    IOSET1 = (1 << 19); // EN = 1
    delay_ms(2);
    IOCLR1 = (1 << 19); // EN = 0
}
 
void lcd_cmd(unsigned int cmd)
{
    unsigned int temp;
    IOCLR1 = 0xFE << 16;
    temp   = ((cmd & 0xF0) << 16);
    IOSET1 = temp;
    IOCLR1 = (1 << 17); // RS = 0 (Command Register)
    IOCLR1 = (1 << 18); // RW = 0
    IOSET1 = (1 << 19); // EN = 1
    delay_ms(2);
    IOCLR1 = (1 << 19); // EN = 0
 
    IOCLR1 = 0xFE << 16;
    temp   = ((cmd & 0x0F) << 20);
    IOSET1 = temp;
    IOCLR1 = (1 << 17); // RS = 0
    IOCLR1 = (1 << 18); // RW = 0
  IOSET1 = (1 << 19); // EN = 1
    delay_ms(2);
    IOCLR1 = (1 << 19); // EN = 0
}
 
void lcd_init(void)
{
    IODIR1 = 0xFE << 16; // Configure P1.17 to P1.23 as output
    lcd_cmd(0x02);       // Return Home (4-bit initialization)
    lcd_cmd(0x28);       // 4-bit mode, 2 lines, 5x7 matrix
    lcd_cmd(0x0E);       // Display ON, Cursor ON
    lcd_cmd(0x01);       // Clear Display
}
 
void lcd_string(char *ptr)
{
    while(*ptr != '\0')
    {
        lcd_data(*ptr);
        ptr++;
    }
}
 
void lcd_integer(int num)
{
    int a[10], i;
    if(num == 0)
        lcd_data('0');
    if(num < 0)
    {
        num = -num;
        lcd_data('-');
    }
    for(i = 0; num > 0; num /= 10, i++)
        a[i] = (num % 10) + 48;
    for(i = i - 1; i >= 0; i--)
        lcd_data(a[i]);
}
 
void lcd_float(float f)
{
    int num, n1;
    if(f == 0)
    {
        lcd_data('0');
        return;
    }
    if(f < 0)
    {
        f = -f;
        lcd_data('-');
    }
    num = f;
    lcd_integer(num);
    lcd_data('.');
    n1 = (f - num) * 100000;
    lcd_integer(n1);
}
