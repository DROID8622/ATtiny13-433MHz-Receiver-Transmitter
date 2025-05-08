# ATtiny13-433MHz-Receiver-Transmitter
 Simple 433mhz or 315mhz remote switch using ATTINY13 with Arduino IDE and MicroCore by MCUdude.
 
  Source code of protocol implementation is from http://skproj.ru/otkryt-shlagbaum-came/
  
  Protocol used is known as protocol 12 from RCSwitch library:
  
  { 320, { 36,  1 }, {  1,  2 }, {  2,  1 }, true }
  
  ( 12 bits, pulselength is 320 microseconds , "1" is 640 low / 320 high , "0" is 640 high / 320 low, pilot period is 11520 (320*36). PROTOCOL is "inverted".
  
  more info here:
  
  https://github.com/sui77/rc-switch/issues/136
  
  https://phreakerclub.com/447

2025.05 update - button function added- in order to fit the sketch - flash microcore bootloader (select "Tools-bootloader-YES" and press "burn bootloader", then select "Tools-bootloader-No Bootloader" and then press "Upload sketch"

Обновление 2025.05 - добавлена функция переключения кнопкой, чтобы скетч влез в ATtiny13 - сначала прошейте бутлоадер - выберите "Tools-bootloader-YES" и нажмите "burn bootloader", далее выберите "Tools-bootloader-No Bootloader" - и после этого нажмите кнопку "Upload sketch"
