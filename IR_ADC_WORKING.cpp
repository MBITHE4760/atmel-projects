
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h >

int ADC_value[8];
int ADC_position;
int adc_flag;
int timer_flag;
int timer_value;
int counter;
int i;
char buffer[200];
int position;
char new_line[10]=" \n";

void ADC_begin()
{
	DDRF=0x00;
	ADCSRA|=(1<<ADEN);
	ADMUX=(1<<REFS0);//set voltage reference
	ADCSRA|=(1<<ADPS2)|(1<<ADPS1);//set prescaler of 64
	ADCSRA|=(1<<ADIE);
	
}

void _uart0_begin()
{
	UBRR0H=0;
	UBRR0L=103;
	UCSR0A|=(1<<RXC0);
	UCSR0B|=(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);
}

void UART0_TRANSMIT(char*request)
{
	while(*request!='\0')
	{
		while(!(UCSR0A&(1<<UDRE0))){}
		UDR0=*request;
		request++;
	}
}

void TIMER_BEGIN()
{
	TCNT0=0;
	OCR0A=75;//for 300us
	TCCR0A=(1<<WGM12);//CTC mode
	TCCR0A=(1<<COM0A1)|(1<<COM0A0);
	TIMSK0=(1<<OCIE0A);//enable timer interrupt
	TCCR0B|=(1<<CS01)|(1<<CS00);//prescaler of 64
}

int main(void)
{
	DDRJ|=(1<<PJ0);
	_uart0_begin();
	TIMER_BEGIN();
	timer_value=0;
	ADC_position=0;
	adc_flag=0;
	ADC_begin();
	sei();
	ADCSRA|=(1<<ADSC);
	while (1)
	{
		_delay_ms(1);
		if (ADC_value[0]<500 && adc_flag == 0)
		{
			PORTJ|=(1<<PJ0);
			adc_flag=1;
			timer_value=0;
			timer_flag=0;
		}
		if ((ADC_value[0] == 500) && (adc_flag == 1) && (timer_flag==1))
		{
			PORTJ&=~(1<<PJ0);
			counter++;
			adc_flag=0;
			timer_flag=0;
		}
		itoa(ADC_value[0], buffer,10);
		_delay_ms(1);

		UART0_TRANSMIT(buffer);
		UART0_TRANSMIT(new_line);
		ADCSRA|=(1<<ADSC);
		
	}
}

ISR(TIMER0_COMPA_vect)
{
	timer_value++;
	if (timer_value>75)
	{
		timer_flag=1;
		timer_value=0;
	}
}

ISR(ADC_vect)
{
	ADC_value[ADC_position]=(ADCL)|(ADCH<<8);
	ADC_position++;
	if(ADC_position==8)
	{
		ADC_position=0;
	}
	ADMUX=(1<<REFS0)|(ADC_position);
	ADCSRA|=(1<<ADSC);
}
