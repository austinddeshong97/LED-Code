//#include <Arduino.h>
#include <EEPROM.h>

#define dataIn 34
#define dataOut 5

void setup(){
  pinMode(dataIn, INPUT);
  pinMode(dataOut, OUTPUT);
}

void loop(){
  float din = digitalRead(dataIn);
  digitalWrite(dataOut, din);
  
}
