#include <Audio.h>
#include "MyDsp.h"

MyDsp myDsp;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;
AudioConnection patchCord0(myDsp,0,out,0);
AudioConnection patchCord1(myDsp,0,out,1);

void setup() {
  Serial.begin(9600);
  AudioMemory(6);
  audioShield.enable();
  
}

void loop() {
  int sensorValue = analogRead(A0);
  audioShield.volume(sensorValue*0.00088);
}

