#include <avr/io.h> //BOiler plate needed
#include <util/setbaud.h>
//Nathaniel Bryant 

static inline void initUART() {
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	#if USE_2X
		UCSR0A |= (1 << U2X0);
	#else  
		UCSR0A &= ~(1 << U2X0); 
	#endif
}






int main() {
	
	DDRD |= ((1<<PD7) | (1<<PD6) | (1<<PD5)); //Turns on the RGB pins
	PORTD &= 1; 
	initUART();//Inits the serial
	while(1) {
		
		
		PORTD &= 1;
		//Need to wait for serial input
		while((UCSR0A>>UDRE0)&1) {
		
		}
		
	}		
	
	
	return 0;	
}



#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	
	DDRB |= 0b11111111;
	DDRD |= 0b11111111;
	
	while(1) {
		PORTD = 0b00110000;
		_delay_ms(500);
		int ii;
		for(ii = 0 ; ii < 8 ; ii++)
		{
			PORTB |= (1 << ii);
			_delay_ms(500);
		}
		_delay_ms(500);
		
		PORTD = 0b001011011;
		
		for(ii = 0 ; ii < 8 ; ii++)
		{
			PORTB &= ~(1 << ii);
			_delay_ms(500);
		}
		_delay_ms(500);
		
		PORTD = 0b11001011;
		
		for(ii = 0 ; ii < 8 ; ii++)
		{
			PORTB ^= (1 << ii);
			_delay_ms(500);
			PORTB ^= (1 << ii);
			_delay_ms(500);
		}
		
		_delay_ms(500);
		
		PORTD = 0b11100010;
		PORTB = 0b11000111;
		_delay_ms(500);
		for(ii = 0 ; ii < 8 ; ii++)
		{
			PORTB = (PORTB << 1);
			_delay_ms(500);
		
		}
		
		
		_delay_ms(500);
	}

	return 1; 
}