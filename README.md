# ATtiny13-433MHz-Receiver-Transmitter
 Simple 433mhz or 315mhz remote switch using ATTINY13 with MicroCore and Arduino IDE by MCUdude.
 
  source of protocol implementation is http://skproj.ru/otkryt-shlagbaum-came/
  
  Protocol used is known as protocol 12 from RCSwitch library:
  
  { 320, { 36,  1 }, {  1,  2 }, {  2,  1 }, true }
  
  ( 12 bits, pulselength is 320 microseconds , "1" is 640 low / 320 high , "0" is 640 high / 320 low, pilot period is 11520 (320*36). PROTOCOL is "inverted".
  
  more info here:
  
  https://github.com/sui77/rc-switch/issues/136
  
  https://phreakerclub.com/447
