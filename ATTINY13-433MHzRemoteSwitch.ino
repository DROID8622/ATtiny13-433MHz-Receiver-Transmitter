/*
  Simple 433mhz or 315mhz remote switch using ATTINY13 (9.6 MHz internal osc. with MicroCore for arduino IDE by MCUdude) and cheap RF modules.
  source of code is
  http://skproj.ru/otkryt-shlagbaum-came/
  Protocol used is known as protocol 12 from RCSwitch library.
  { 320, { 36,  1 }, {  1,  2 }, {  2,  1 }, true }
  ( 12 bits, pulselength is 320 microseconds , "1" is 640 low / 320 high , "0" is 640 high / 320 low, pilot period is 11520 (320*36).
  PROTOCOL is "inverted".
  more info:
  https://github.com/sui77/rc-switch/issues/136
  https://phreakerclub.com/447
*/

#define rxPin PB1 //receiver
#define txPin PB3 // transmitter
#define ledPin PB4 // LED or relay
#define F_CPU 9600000UL
volatile static int lastCode = 0;

void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(INT0, grab, CHANGE); //INT0 PB1 hardware interrupt
  interrupts();
}

void loop() {
  //codes from 0 to 4095 can be used
  if (lastCode == 2211) {
    // Serial.println(lastCode & 0xfff);
    digitalWrite(ledPin, HIGH);
      lastCode = 0;
    //response - can be deleted if no need
    delay(500);
    SendCame4(2266);
  }
  if (lastCode == 2200) {
    // Serial.println(lastCode & 0xfff);
    digitalWrite(ledPin, LOW);
      lastCode = 0;
    //response:
    delay(500);
    SendCame4(2255);
  }

}

//send function:
void SendCame4(int Code) {
  //noInterrupts();
  for (int j = 0; j < 10; j++) { // send 10 times for better receiving
    // pulse length is shorter than should be (300 instead of 320 and so on) - my Attiny13 a little slow
    digitalWrite(txPin, HIGH);
    delayMicroseconds(300);
    digitalWrite(txPin, LOW);
    for (int i = 12; i > 0; i--) {
      byte b = bitRead(Code, i - 1); // send every bit of code
      if (b) {
        digitalWrite(txPin, LOW); // 1
        delayMicroseconds(600);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(300);
      }
      else {
        digitalWrite(txPin, LOW); // 0
        delayMicroseconds(300);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(600);
      }
    }
    digitalWrite(txPin, LOW);
    delayMicroseconds(10800);
  }
 // interrupts();
}

// receiving code below:
#define MAX_DELTA 200 // impulse lenght may vary of battery and so on, max deviation is 200
volatile unsigned long prevtime;
volatile unsigned int lolen, hilen, rxstate;
volatile static byte cameCounter = 0;    // count of bits stored
volatile static int cameCode = 0;       // code itself

boolean CheckValue(unsigned int base, unsigned int value) {
  return ((value == base) || ((value > base) && ((value - base) < MAX_DELTA)) || ((value < base) && ((base - value) < MAX_DELTA)));
}

void grab() {
  rxstate = digitalRead(rxPin);
  if (rxstate == HIGH)
    lolen = micros() - prevtime;
  else
    hilen = micros() - prevtime;
  prevtime = micros();

  if (rxstate == LOW) {
    if (CheckValue(320, hilen) && CheckValue(640, lolen)) {
      // valid 1
      cameCode = (cameCode << 1) | 1;
      cameCounter++;
    }
    else if (CheckValue(640, hilen) && CheckValue(320, lolen)) {
      // valid 0
      cameCode = (cameCode << 1) | 0;
      cameCounter++;
    }
    else cameCounter = 0;
  } else if (lolen > 1000 && (cameCounter == 12 || cameCounter == 13) && ((cameCode & 0xfff) != 0xfff)) {
    lastCode = cameCode & 0xfff;
    cameCounter = 0;
    cameCode = 0;
  }
}
