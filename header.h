#ifndef __HEADER_H__
#define __HEADER_H__
 
#include <LPC21xx.H>
 
#define RED_LED    (1<<17)
#define YELLOW_LED (1<<18)
#define GREEN_LED  (1<<19)
#define buz        (1<<21)
 
#define sw1        ((IOPIN0>>14)&1)
#define sw2        ((IOPIN0>>15)&1)
 
typedef unsigned int  u32;
typedef signed int    s32;
typedef unsigned char u8;
typedef signed char   s8;
 
typedef struct {
    u32 id;
    u32 rtr;
    u32 dlc;
    u32 byteA;
    u32 byteB;
} CAN1;
 
/* Timer0 Delay Prototypes */
void delay_sec(unsigned int sec);
void delay_ms(unsigned int m_s);
void delay_us(unsigned int ms);
 
/* UART0 Driver Prototypes */
void uart0_init(unsigned int baud);
void uart0_tx(unsigned char data);
unsigned char uart0_rx(void);
void uart0_tx_string(char *ptr);
void uart0_binary(int num);
void uart0_integer(int num);
void uart0_float(float f);
 
/* 16x2 LCD Driver Prototypes */
void lcd_init(void);
void lcd_cmd(unsigned int cmd);
void lcd_data(unsigned char data);
void lcd_string(char *ptr);
void lcd_integer(int num);
void lcd_float(float f);
 
/* ADC0 & Ultrasonic Sensor Prototypes */
void adc_init(void);
u32 adc_read(u8 ch_num);
void ultrasonic_init(void);
u32 ultrasonic_read(void);
 
/* Servo PWM4 & External Interrupt EINT0 Prototypes */
void servo_init(void);
void servo_0deg(void);
void servo_90deg(void);
void servo_180deg(void);
void EINT0_Handler(void) __irq;
void config_for_eint(void);
 
/* I2C0, EEPROM, CAN1 & ESP-01 Wi-Fi Prototypes */
void i2c_init(void);
void i2c_write(u8 sa, u8 mr, u8 data);
u8 i2c_read(u8 sa, u8 mr);
void eeprom(u8 t);
void can1_init(void);
void can1_tx(CAN1 v);
void can1_parameter(u8 t, u8 w, u8 d);
void wifi_connect(void);
void esp_read(void);
 
#endif
