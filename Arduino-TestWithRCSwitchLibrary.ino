/*
  Example for different sending methods

  https://github.com/sui77/rc-switch/

*/

#include <RCSwitch.h>
#define PERIOD_1 4000
int toggle = 0;
unsigned long timer_1;
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  mySwitch.enableTransmit(3);
  mySwitch.enableReceive(0);
  mySwitch.setProtocol(12);
  mySwitch.setRepeatTransmit(8);
}

void loop() {
  if (mySwitch.available()) {
    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.println( mySwitch.getReceivedProtocol() );
    mySwitch.resetAvailable();
  }

  if (millis() - timer_1 >= PERIOD_1) {
    timer_1 = millis();

    if (toggle == 1) {
      mySwitch.send(2211, 12); //send ON
      toggle = 0;
    }
    else {
      mySwitch.send(2200, 12); //send OFF
      toggle = 1;
    }
  }
}
