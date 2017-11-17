/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <wiringPi.h>

#define IRLEDpin  2              //the arduino pin connected to IR LED to ground. HIGH=LED ON
#define OnButtonPin 3
#define OffButtonPin 4
#define ToggleButtonPin 5 
#define BITtime   562           //length of the carrier bit in microseconds
#define LEDpin 0
#define socketPort 51717

int onButtonState = 0;
int offButtonState = 0; 
int toggleButtonState = 0;

unsigned long toggleCode  = 0b00100000110111110000111011110001;
unsigned long onCode      = 0b00100000110111111000111001110001;
unsigned long offCode     = 0b00100000110111110100111010110001;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// Ouput the 38KHz carrier frequency for the required time in microseconds
// This is timing critial and just do-able on an Arduino using the standard I/O functions.
// If you are using interrupts, ensure they disabled for the duration.
void IRcarrier(unsigned int IRtimemicroseconds) {
	int i;
	for(i=0; i < (IRtimemicroseconds / 26); i++) {
		digitalWrite(LEDpin, HIGH);   //turn on the IR LED
		
		//NOTE: digitalWrite takes about 3.5us to execute, so we need to factor that into the timing.
		delayMicroseconds(13);          //delay for 13us    (9us + digitalWrite), half the carrier frequnecy
		digitalWrite(LEDpin, LOW);    //turn off the IR LED
		delayMicroseconds(13);          //delay for 13us (9us + digitalWrite), half the carrier frequnecy
    }
}

//Sends the IR code in 4 byte NEC format
void IRsendCode(unsigned long code) {
	//send the leading pulse
	IRcarrier(9000);            //9ms of carrier
	delayMicroseconds(4500);    //4.5ms of silence

	//send the user defined 4 byte/32bit code
	int i;
	for (i=0; i<32; i++) {           	//send all 4 bytes or 32 bits
		IRcarrier(BITtime);               	//turn on the carrier for one bit time
		if (code & 0x80000000)            	//get the current bit by masking all but the MSB
			delayMicroseconds(3 * BITtime); //a HIGH is 3 bit time periods
		else
			delayMicroseconds(BITtime);     //a LOW is only 1 bit time period
		code<<=1;                        	//shift to the next bit for this byte
	}
	IRcarrier(BITtime);                 	//send a single STOP bit.
}
