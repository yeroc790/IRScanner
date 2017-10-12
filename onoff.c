//*****************************************
// NEC (Japanese) Infrared code sending library for the Arduino
// Send a standard NEC 4 byte protocol direct to an IR LED on the define pin
// Assumes an IR LED connected on I/O pin to ground, or equivalent driver.
// Tested on a Freetronics Eleven Uno compatible
// Written by David L. Jones www.eevblog.com
// Youtube video explaining this code: http://www.youtube.com/watch?v=BUvFGTxZBG8
// License: Creative Commons CC BY
//*****************************************

#define IRLEDpin  2              //the arduino pin connected to IR LED to ground. HIGH=LED ON
#define OnButtonPin 3
#define OffButtonPin 4
#define ToggleButtonPin 5
#define BITtime   562            //length of the carrier bit in microseconds

int onButtonState = 0;
int offButtonState = 0;
int toggleButtonState = 0;

unsigned long toggleCode  = 0b00100000110111110000111011110001;
unsigned long onCode      = 0b00100000110111111000111001110001;
unsigned long offCode     = 0b00100000110111110100111010110001;

// NOTE: send the binary as is, it's translated from the hex code
// NOTE: the binary segments are in order of 1 2 4 8, not 8 4 2 1

//  discrete on:
//    hex:    04-FB-71-8E
//    binary: 0010 0000 (40) | 1101 1111 (BF) | 1000 1110 (17) | 0111 0001 (E8)
//            ^ this pair       ^ is the inverse      ^                 ^

//  discrete off:
//    hex:    04-FB-72-8D
//    binary: 0010 0000 (40) | 1101 1111 (BF) | 0100 1110 (27) | 1011 0001 (D8)
//              ^      address        ^           ^       data          ^

//  toggle on/off:
//    hex:    04-FB-70-8F
//    binary: 0010 0000 (40) | 1101 1111 (BF) | 0000 1110 (07) | 1111 0001 (F8)

void setup()
{
  IRsetup();                          //Only need to call this once to setup
}

void IRsetup(void)
{
  pinMode(OnButtonPin, INPUT_PULLUP);
  pinMode(OffButtonPin, INPUT_PULLUP);
  pinMode(ToggleButtonPin, INPUT_PULLUP);
  pinMode(IRLEDpin, OUTPUT);
  digitalWrite(IRLEDpin, LOW);    //turn off IR LED to start
}

// Ouput the 38KHz carrier frequency for the required time in microseconds
// This is timing critial and just do-able on an Arduino using the standard I/O functions.
// If you are using interrupts, ensure they disabled for the duration.
void IRcarrier(unsigned int IRtimemicroseconds)
{
  for(int i=0; i < (IRtimemicroseconds / 26); i++)
    {
    digitalWrite(IRLEDpin, HIGH);   //turn on the IR LED
    //NOTE: digitalWrite takes about 3.5us to execute, so we need to factor that into the timing.
    delayMicroseconds(9);          //delay for 13us (9us + digitalWrite), half the carrier frequnecy
    digitalWrite(IRLEDpin, LOW);    //turn off the IR LED
    delayMicroseconds(9);          //delay for 13us (9us + digitalWrite), half the carrier frequnecy
    }
}

//Sends the IR code in 4 byte NEC format
void IRsendCode(unsigned long code)
{
  //send the leading pulse
  IRcarrier(9000);            //9ms of carrier
  delayMicroseconds(4500);    //4.5ms of silence

  //send the user defined 4 byte/32bit code
  for (int i=0; i<32; i++)            //send all 4 bytes or 32 bits
    {
    IRcarrier(BITtime);               //turn on the carrier for one bit time
    if (code & 0x80000000)            //get the current bit by masking all but the MSB
      delayMicroseconds(3 * BITtime); //a HIGH is 3 bit time periods
    else
      delayMicroseconds(BITtime);     //a LOW is only 1 bit time period
     code<<=1;                        //shift to the next bit for this byte
    }

  IRcarrier(BITtime);                 //send a single STOP bit.
}

void loop()
{
  onButtonState = digitalRead(OnButtonPin);
  offButtonState = digitalRead(OffButtonPin);
  toggleButtonState = digitalRead(ToggleButtonPin);

  if (offButtonState == LOW) {
    IRsendCode(offCode);
  } else if (onButtonState == LOW) {
    IRsendCode(onCode);
  } else if (toggleButtonState == LOW) {
    IRsendCode(toggleCode);
  }
  delay(1000);
}
