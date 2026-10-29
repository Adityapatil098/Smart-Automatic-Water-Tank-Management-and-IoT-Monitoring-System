#include <LPC21xx.H>
#include "header.h"
 
void wifi_connect(void) // P0.0 -> TXD0, P0.1 -> RXD0
{
    uart0_tx_string("AT\r\n");
    delay_ms(2000);
    uart0_tx_string("AT+CWMODE=1\r\n");
    delay_ms(2000);
    uart0_tx_string("AT+CWJAP=\"Samsung\",\"91919191\"\r\n");
    delay_ms(20000);
    uart0_tx_string("AT+CIFSR\r\n");
    delay_ms(2000);
    uart0_tx_string("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    delay_ms(5000);
    uart0_tx_string("AT+CIPSEND=122\r\n");
    delay_ms(20000);
    uart0_tx_string("GET /update?api_key=K0KJB6VB538JT2WJ&field1=50&field2=120&field3=28&field4=1 HTTP/1.1\r\n");
    delay_ms(5000);
    uart0_tx_string("Host:api.thingspeak.com\r\n\r\n");
    delay_ms(5000);
    uart0_tx_string("Connection:close\r\n\r\n");
    delay_ms(8000);
}
 
void esp_read(void)
{
    unsigned char ch;
    while(1)
    {
        ch = uart0_rx();
        uart0_tx(ch);
    }
}
