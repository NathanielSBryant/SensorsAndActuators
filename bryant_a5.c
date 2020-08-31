#include <avr/io.h>
#include <util/setbaud.h> //Needed for serial
#include "i2c.h"
#include <util/delay.h>


#define ADXL345_ADDRESS_W 0b10100110
#define ADXL345_ADDRESS_R 0b10100111

static inline void initI2CForMe() {
	
	initI2C();
	//Lets get the init done
	i2cStart();//Starting
	i2cSend(ADXL345_ADDRESS_W);//Writing to the slave
	i2cSend(0x2D);//Tell where to write 
	i2cSend(1<<3);//Write this to the power control
	i2cStop();//All done 
	
	i2cStart();//Starting
	i2cSend(ADXL345_ADDRESS_W);//Lets write 
	i2cSend(0x38);//Write to the FIFO_CTRL register
	i2cSend(1<<7);//Set streaming mode 
	i2cStop();
	
}


//Just inits the UART
static inline void initUART() {
	//Set BUARD rate
	UBRR0H = UBRRH_VALUE;
	//COnt
	UBRR0L = UBRRL_VALUE;
	//Turn on Transever and reciver
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	//Set size to 8 bits
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	//Set bit stop bit ammount to 1 
	UCSR0C &= ~(1 << USBS0);

	//Autosets the BAUD rate relitive to clock speed
	#if USE_2X
		UCSR0A |= (1 << U2X0);
	#else  
		UCSR0A &= ~(1 << U2X0); 
	#endif
}


//This is a helper function that reads the register for me
char readRegister(int value) {
	
	i2cStart();//Telling the slave to pay attention
	i2cSend(ADXL345_ADDRESS_W);//Writing 
	i2cSend(value);//This register
	i2cStart();//Start sending me the data
	i2cSend(ADXL345_ADDRESS_R);//I'm readinging 
	uint8_t registerValue1 = i2cReadAck();//Saving the info
	uint8_t registerValue2 = i2cReadNoAck();//Saving the info
	i2cStop();//Thank you
	
	return ((~((registerValue1 << 8) | registerValue2)+1)%10)+'0';
}

//Sends all values given to the serial
void sendToSerial(char value){
	UDR0 =  value ; 
	//Waits to make sure it got sent off 
	while((~(UCSR0A>>UDRE0))&1) {
	}
	
}


//Main 
int main() {
	
	
	
	initI2CForMe();//what it says on the tin
	initUART();//Inits the serial

	
	//Awsome lets get started 
	while(1){
		
		//First step is to read the data
		//Read in all values of x y and z
		char x_register_value = readRegister(0x32);//X register1 

		char y_register_value = readRegister(0x34);//Y regiser 
		char z_register_value = readRegister(0x36);//Z register
		//Now send it out to the serial
		
		
		sendToSerial(x_register_value);
		sendToSerial(y_register_value);		
		sendToSerial(z_register_value);	
		sendToSerial('\n');
	
		//ONE MORE TIME!
		_delay_ms(500);
		
	}
	return 1;
}