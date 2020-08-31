#include <avr/io.h> //BOiler plate needed
#include <util/delay.h>
#include <avr/interrupt.h>
//Nathaniel Bryant 




//Inits the ADC for useage 
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

//A command that returns the adc value 
int readADC() {

	while((ADCSRA>>ADSC)&1){
		} //Waiting till ADC is done 
	uint16_t value = ADC;//Read that value
	return value;	

}

//Inits the interupt system probably don't need it
static inline void initEIMSK() { 
	DDRD |= (1<<PD2); //Turn on pull up reistor
	PORTD |= (1<<PD2); // cont 
	EIMSK |= (1 << INT0);//Setting PD2 as an interupt pin
	EICRA |= (1 << ISC01) | (0 << ISC00);//Setting  the sense controll to the falling edge
	sei(); //Activate interupts
	
} 

//Jsut a function call to wait for a button
static void buttonWait()
{
	while((PIND>>PD1)&1){
	}
}


int main(void) {
	
	
	DDRD  = (1<<PD6);//Sets all pins to be read except PD6
	
	
	//Init Sections 
	initADC();
	initEIMSK();
	//Value Setction
    double replenishValue = 0.001; //This is the value that if the weight changes it triggers the system`
	double takeOutValue = 0.05; //This is the value that will not trigger the replenish action
	double flowRate; //This value is used to determine the flow rate of the solinoid valve
	double currentWeight; // The currentWeight of the system
	double primaryRatio;//The percentage of the weight of the primary matieral
	double seconMaterial;//The percentage of the weight of secondary mateiral
	double primeMaterialWeight;//THe weight of the main material 
	double secondMaterialWeight; // The weight of the added material
	
	//Lets do the calcibration stuff first
	
	//Reads the weight of the system of containers and everything
	
	currentWeight = readADC;
	
	//Next is to turn on the solonoid for a time to get the flow rate
	PORTD ^= (1<<PD6);//Turn on the solonid
	_delay_ms(500);
	PORTD ^= (1<<PD6);
	
	//Gets the changed weight 
	flowRate = (readADC()-currentWeight)/500.0; //Calculates the flow rate 
	
	
	
	//Calibration done now for setup
	
	
	buttonWait();
	
	//Calculate the weight of the main material added 
	primeMaterialWeight = (readADC-currentWeight);
	currentWeight = readADC();
	
	buttonWait();
	//Calculate the weight of the secondary material 
	secondMaterialWeight = (readADC()-currentWeight);
	currentWeight = readADC();
	
	//Calcautes the ratios of both materials
	primaryRatio  = primeMaterialWeight/secondMaterialWeight;
	secondMaterialRatio = secondMaterialWeight/primeMaterialWeight;
	
	
	//Now lets get to the good stuff
	while(1)
	{
		double checkWeight = readADC();
		
		double difference = -1*(checkWeight-currentWeight)/currentWeight;
		//Do nothing if nothing has changed
		if(difference == 0) {
			
			
			
		}
		//When we take something out
		else if(difference > takeOutValue) {
			//Remove the amount of primary matieral from the system
			primeMaterialWeight -= (difference*primaryRatio);
			//Remove the amount of secondary material from the system
			secondMaterialWeight -= (difference*secondMaterialRatio);
			currentWeight = checkWeight; //Update the weight and done
		}
		//When its time to replenish the system 
		else if(difference > replenishValue)
		{
			
			int valveOpenTime = (difference/flowRate);//Calculates how long the valve should be open 
			PORTD ^= (1<<PD6);//Toggle the valve
			_delay_ms(valveOpenTime);//Keeps the valve open for time needed
			PORTD ^= (1<<PD6);//Toggle the valve
			
		}
		_delay_ms(1000);//Wait about a second for each reading to save power 
		
		//ONE MORE TIME
	}
	
	return 0; //How did you get here?
}