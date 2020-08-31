#include <avr/io.h> //BOiler plate needed

//Nathaniel Bryant 


static inline void initADC(void) 
{
	
	ADMUX 	|= (1<<REFS0);	//Giving power to the ADC
	ADMUX	|= (1<<MUX1); 	//Turn the mux for pin ADC3
	ADMUX	|= (1<<MUX0); 	//Cont
	ADCSRA 	|= (1<<ADPS0) ;	//Setting division factor to 8
	ADCSRA	|= (1<<ADPS1); 	//Cont
	ADCSRB	|= (1<<ADTS0);	//Turning off free running mode
	ADCSRA  |= (1<<ADEN);	//Turning on the ADC
}

static inline void initPWM(void) {
    TCCR0A 	|= (1 << WGM00);   	// set fast PWM mode, pt. 1
    TCCR0A 	|= (1 << WGM01);	// set fast PWM mode, pt. 2
	TCCR0B	|= (1 << CS01);		// set clock speed
	TCCR0B	|= (1 << CS01);		// set clock speed
	TCCR0A	|= (1 << COM0A1);	// turn on PMW
	DDRD	|= (1 << PD6); 		// Set PD6 to output 
}
int main(void) {
	
	initADC(); //Inits the ADC 
	initPWM(); //Inits the PWM
	uint16_t adcValue;//Value to save for voltage 
	
	
	
	while(1)
	{	

		ADCSRA	|= (1<<ADSC);//Turning on the ADC read
		
		while((ADCSRA>>ADSC)&1){
		} //Waiting till ADC is done 
		adcValue = ADC;//Read that value 
		
		OCR0A = adcValue*55;//Convert to a scale of 255(55*5)=255
		
		
		
	}

	return 1;
}