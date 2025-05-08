#define rxPin PB1      //receiver
#define txPin PB3      // transmitter
#define buttonPin PB2  // switch button
#define ledPin PB4     // LED or relay
volatile static int lastCode = 0; // received code
bool bState = 0; //button state
bool lState = 0; //led state
void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(INT0, grab, CHANGE);  //INT0 PB1 hardware interrupt
  interrupts();
}

void loop() {
// button is connected to PB2 and GND
  bState = digitalRead(PB2);
  if (bState == LOW) {
    lState = !lState;
    digitalWrite(ledPin, lState);
       delay(100);
    if (lState==1) {
       SendCame4(2233);
    } else {
       SendCame4(2222);
    }
  }

  //codes from 0 to 4095 can be used
  if (lastCode == 2211) {
    digitalWrite(ledPin, HIGH);
    lastCode = 0;
    SendCame4(2233); //response - can be deleted if no need
  }
  if (lastCode == 2200) {
    digitalWrite(ledPin, LOW);
    lastCode = 0;
    SendCame4(2222); //response - can be deleted if no need
  }
}

//send function:
void SendCame4(int Code) {
  //noInterrupts();
  for (int j = 0; j < 4; j++) {  // send 10 times for better receiving
    // pulse length can be shorter than should be (300 instead of 320 and so on) - my Attiny13 a little slow
    digitalWrite(txPin, HIGH);
    delayMicroseconds(320);
    digitalWrite(txPin, LOW);
    for (int i = 12; i > 0; i--) {
      byte b = bitRead(Code, i - 1);  // send every bit of code
      if (b) {
        digitalWrite(txPin, LOW);  // 1
        delayMicroseconds(640);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(320);
      } else {
        digitalWrite(txPin, LOW);  // 0
        delayMicroseconds(320);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(640);
      }
    }
    digitalWrite(txPin, LOW);
    delayMicroseconds(11520);
  }
  // interrupts();
}


// receiving code below:

#define MAX_DELTA 200  // impulse lenght may vary of battery and so on, max deviation is 200
volatile unsigned long prevtime;
volatile unsigned int lolen, hilen, rxstate;
volatile static byte cameCounter = 0;  // count of bits stored
volatile static int cameCode = 0;      // code itself

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
    } else if (CheckValue(640, hilen) && CheckValue(320, lolen)) {
      // valid 0
      cameCode = (cameCode << 1) | 0;
      cameCounter++;
    } else cameCounter = 0;
  } else if (lolen > 1000 && (cameCounter == 12 || cameCounter == 13) && ((cameCode & 0xfff) != 0xfff)) {
    lastCode = cameCode & 0xfff;
    cameCounter = 0;
    cameCode = 0;
  }
}
