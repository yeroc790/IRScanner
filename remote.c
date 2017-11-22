#include "functions.c"

int main () {
	/* GPIO STARTUP */
	wiringPiSetup();
	pinMode(LEDpin, OUTPUT);
	digitalWrite(LEDpin, LOW);
	
	pinMode(OnButtonPin, INPUT);
	pullUpDnControl(OnButtonPin, PUD_UP);
	
	pinMode(OffButtonPin, INPUT);
	pullUpDnControl(OffButtonPin, PUD_UP);
	
	while(1) {
		/* READ INPUT STATES */
		onButtonState = digitalRead(OnButtonPin);
		offButtonState = digitalRead(OffButtonPin);

		/* SEND SIGNAL */
		if (onButtonState == LOW) {
			IRsendCode(onCode);
			delay(1000);
		} else if (offButtonState == LOW) {
			IRsendCode(offCode);
			delay(1000);
		}
	}
}




