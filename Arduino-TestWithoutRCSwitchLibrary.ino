
#define txPin 3 //
#define rxPin 2 //


#define PERIOD_1 4000
int toggle = 0;
unsigned long timer_1;
void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  Serial.begin(9600);  //
  Serial.println("Came started");

  attachInterrupt(0, grab, CHANGE);
  interrupts();
}

volatile static long lastCode = 0;

void loop() {

  if (millis() - timer_1 >= PERIOD_1) {
    timer_1 = millis();
    if (toggle == 1) {
      SendCame4(2211);
      toggle = 0;
    }
    else {
      SendCame4(2200);
      toggle = 1;
    }
  }

  if (lastCode > 0) {
    Serial.println(lastCode & 0xfff);
    lastCode = 0;
  }
}

void SendCame4(long Code) {
  noInterrupts();
  for (int j = 0; j < 10; j++) { // посылку посылаем 10 раза подряд.
    // время стартового импульса
    digitalWrite(txPin, HIGH);
    delayMicroseconds(320);
    digitalWrite(txPin, LOW);
    for (int i = 12; i > 0; i--) {
      byte b = bitRead(Code, i - 1); // побитово перебираем и посылаем код
      if (b) {
        digitalWrite(txPin, LOW); // 1
        delayMicroseconds(640);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(320);
      }
      else {
        digitalWrite(txPin, LOW); // 0
        delayMicroseconds(320);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(640);
      }
    }
    digitalWrite(txPin, LOW);
    delayMicroseconds(11520);
  }
  interrupts();
}


#define MAX_DELTA 200
// длительность импульсов может плавать в зависимости от заряда батареии - макс допуск 200 мкс
boolean CheckValue(unsigned int base, unsigned int value)
{
  return ((value == base) || ((value > base) && ((value - base) < MAX_DELTA)) || ((value < base) && ((base - value) < MAX_DELTA)));
}
volatile unsigned long prevtime;
volatile unsigned int lolen, hilen, state;
volatile static byte cameCounter = 0;    // count of bits stored
volatile static long cameCode = 0;       // code itself

void grab() {
  state = digitalRead(rxPin);
  if (state == HIGH)
    lolen = micros() - prevtime;
  else
    hilen = micros() - prevtime;
  prevtime = micros();

  if (state == LOW)
  {
    // последовательность закончилась
    if (CheckValue(320, hilen) && CheckValue(640, lolen)) // valid 1
    {
      cameCode = (cameCode << 1) | 1;
      cameCounter++;
    }
    else if (CheckValue(640, hilen) && CheckValue(320, lolen)) // valid 0
    {
      cameCode = (cameCode << 1) | 0;
      cameCounter++;
    }
    else cameCounter = 0;
  } else if (lolen > 1000 &&
             (cameCounter == 12 || cameCounter == 13) &&
             ((cameCode & 0xfff) != 0xfff))
  {
    lastCode = cameCode & 0xfff;
    cameCounter = 0;
    cameCode = 0;
  }
}
