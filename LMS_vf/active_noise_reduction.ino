#include <Audio.h>
#include "MyDsp.h"

MyDsp myDsp;
AudioOutputI2S out;
AudioControlSGTL5000 audioShield;
AudioConnection patchCord0(myDsp,0,out,0);

void setup() {
  Serial.begin(9600);
  AudioMemory(6);
  audioShield.enable();
  audioShield.volume(0.3);
  audioShield.micGain(10);
  
}

void loop() {
}

