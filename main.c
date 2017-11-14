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
#define ToggleButtonPin 3
#define BITtime   562           //length of the carrier bit in microseconds
#define LEDpin 0

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

int main(int argc, char *argv[])
{
     /* GPIO STARTUP */
	wiringPiSetup();
	pinMode(LEDpin, OUTPUT);
	digitalWrite(LEDpin, LOW);
	
	pinMode(ToggleButtonPin, INPUT);
	pullUpDnControl(ToggleButtonPin, PUD_UP);
	
	/* Socket setup */
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if (argc < 2) {
	 fprintf(stderr,"ERROR, no port provided\n");
	 exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		  sizeof(serv_addr)) < 0) 
		  error("ERROR on binding");
	
	while(1) {
		/* READ INPUT STATES */
		toggleButtonState = digitalRead(ToggleButtonPin);

		/* SEND SIGNAL */
		/*if (offButtonState == LOW) {
			IRsendCode(offCode);
		} else if (onButtonState == LOW) {
			IRsendCode(onCode);
		} else if (toggleButtonState == LOW) {
			IRsendCode(toggleCode);
		}*/
		if (toggleButtonState == LOW) {
			IRsendCode(toggleCode);
			delay(1000);
		}
		
		listen(sockfd,5);
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, 
				 (struct sockaddr *) &cli_addr, 
				 &clilen);
		if (newsockfd < 0) 
		  error("ERROR on accept");
		bzero(buffer,256);
		n = read(newsockfd,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		//printf("Here is the message: %s\n",buffer);
		n = write(newsockfd,"I got your message",18);
		if (n < 0) error("ERROR writing to socket");
		
		if(strcmp(buffer,"on") == 0) {
			printf("sending on code\n");
			IRsendCode(onCode);
			delay(1000);
		} else if (strcmp(buffer,"off") ==0) {
			printf("sending off code\n");
			IRsendCode(offCode);
			delay(1000);
		} else if (strcmp(buffer,"toggle") == 0) {
			printf("sending toggle code\n");
			IRsendCode(toggleCode);
			delay(1000);
		} else {
			printf("unaccepted command\n");
		}
	}
	
	/* need to make this run on force quit */
	close(newsockfd);
	close(sockfd);
	return 0;
}
