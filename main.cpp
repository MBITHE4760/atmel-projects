/*
 * http.cpp
 *
 * Created: 3/26/2021 3:02:40 PM
 * Author : mbithe
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

char buffer[200];
int position=0;

//USING GET
char AT[]="AT\r";
char CONTYPE[]="AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r";
char APN[]="AT+SAPBR=3,1,\"APN\",\"safaricom\"\r";
char USER[]="AT+SAPBR=3,1,\"USER\",\"saf\"\r";
char PWD[]="AT+SAPBR=3,1,\"PWD\",\"data\"\r";
char open_GPRS[]="AT+SAPBR=1,1\r";
char query_GPRS[]="AT+SAPBR=2,1\r";
char http_init[]="AT+HTTPINIT\r";
char http_para[]="AT+HTTPPARA=\"CID\",1\r";
char URL[]="AT+HTTPPARA=\"URL\",\"www.fedha.ke/http/insert.php?count=30\"\r";
char start_GET[]="AT+HTTPACTION=0\r";
char http_read[]="AT+HTTPREAD\r";
char http_term[]="AT+HTTPTERM\r";
char close_GPRS[]="AT+SAPBR=0,1\r";

void UART0_BEGIN()
{
UBRR0H=0;
UBRR0L=103;
//UCSR0A|=(1<<RXC0);
UCSR0B|=(1<<RXEN0)|(1<<TXEN0)|(1<<TXCIE0);
UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);	
}

void UART0_TRANSMIT(char*request)
{
	while (*request!='\0')
	{
		while (!(UCSR0A&(1<<UDRE0))){}
			UDR0=*request;
			request++;
	}
}

int main(void)
{
  UART0_BEGIN();
  sei();
    while (1) 
    {
		_delay_us(1);
		UART0_TRANSMIT(AT);
		 _delay_ms(1000);
		UART0_TRANSMIT(CONTYPE);
		 _delay_ms(1000);
		UART0_TRANSMIT(APN);
		 _delay_ms(1000);
		UART0_TRANSMIT(USER);
		 _delay_ms(1000);
		UART0_TRANSMIT(PWD);
		 _delay_ms(1000);
		UART0_TRANSMIT(open_GPRS);
		 _delay_ms(1000);
		UART0_TRANSMIT(query_GPRS);
		 _delay_ms(1000);
		UART0_TRANSMIT(http_init);
		 _delay_ms(1000);
		UART0_TRANSMIT(http_para);
		 _delay_ms(1000);
		UART0_TRANSMIT(URL);
		 _delay_ms(1000);
		UART0_TRANSMIT(start_GET);
		 _delay_ms(1000);
		UART0_TRANSMIT(http_read);
		 _delay_ms(5000);
		UART0_TRANSMIT(http_term);
		 _delay_ms(1000);
		UART0_TRANSMIT(close_GPRS);
    }
}

// ISR(USART0_TX_vect)
// {
// 	buffer[position]=UDR0;
// 	position++;
// }

