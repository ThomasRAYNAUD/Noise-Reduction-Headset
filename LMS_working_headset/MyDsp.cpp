#include "MyDsp.h"
#define NUM_COEFFICIENTS 2
#define AUDIO_OUTPUTS 2
#define MULT_16 32767
#define DIV_16 0.0000305185
#define BUFFER_SIZE NUM_COEFFICIENTS

MyDsp::MyDsp() : AudioStream(AUDIO_OUTPUTS, new audio_block_t*[AUDIO_OUTPUTS]),
                noise(), // OK
                oneZero(), // OK
                mu(0.001), // OK
                adaptSignal(0.0), // OK
                error(0), // OK
                output_H(0)
{
    for (int i = 0; i < NUM_COEFFICIENTS; i++) {
        coefficients[i] = 0.0 ;
        input[i] = 0.0 ;
    }
}

MyDsp::~MyDsp() {}

void MyDsp::LMS(float sample, int i, int channel, audio_block_t** outBlock , audio_block_t** inBlock) {
  // push le sample > dans input > input[NUM_COEFFICIENTS - 1]
  pushInput(sample); // OK
  outputSys(i,channel,outBlock,inBlock); // OK
  // calculer la sortie du système H > somme
  adaptativeFilter(); // "for" à revoir dans le doute
  // calculer l'erreur à la sortie des deux blocs
  outputError();
  // calculer les nouveaux coefs
  correctionCoef();
}

void MyDsp::pushInput(float sample) {
    for (int i = 0; i < (NUM_COEFFICIENTS - 1); i++) {
        input[i] = input[i + 1];
    }
    input[NUM_COEFFICIENTS - 1] = sample ;
}

void MyDsp::outputSys(int i, int channel, audio_block_t** outBlock , audio_block_t** inBlock) {
  float headset = input[NUM_COEFFICIENTS - 1] ;
  //Serial.println(input[NUM_COEFFICIENTS - 1]);
  int16_t val = headset * MULT_16;
  outBlock[channel]->data[i] = val; // i, 
  transmit(outBlock[channel], channel);
  
  if(inBlock[channel]){
     output_H = inBlock[channel]->data[i]*DIV_16;
     //Serial.println(output_H);
  }
  
  //output_H = 
  //output_H = input[NUM_COEFFICIENTS - 1] * 0.44 ;
}

void MyDsp::adaptativeFilter() {
  adaptSignal = 0 ; // si pas de reset erreur dans la console "OVF"
  for (int v = 0; v < NUM_COEFFICIENTS; v++) {
      adaptSignal += input[NUM_COEFFICIENTS - 1] * coefficients[v];
  }
}

void MyDsp::outputError() {
  //Serial.println(output_H);
  //Serial.print("      ");
  //Serial.println(adaptSignal);
  error = output_H - adaptSignal ;
  Serial.println(error);
}

void MyDsp::correctionCoef() {
    for (int k = 0; k < NUM_COEFFICIENTS; k++) {
        coefficients[k] = coefficients[k] + (mu * error * input[NUM_COEFFICIENTS - 1]) ;
    }
}

void MyDsp::update(void) {
    audio_block_t *outBlock[AUDIO_OUTPUTS];
    audio_block_t *inBlock[AUDIO_OUTPUTS];
    for (int channel = 0; channel < AUDIO_OUTPUTS; channel++) {
        inBlock[channel] = receiveReadOnly(channel);
        outBlock[channel] = allocate();
        if (outBlock[channel]) {
            for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
                float currentSample = oneZero.tick(noise.tick()) * 0.5;
                LMS(currentSample, i, channel, outBlock, inBlock);
            }
            if(inBlock[channel]) release(inBlock[channel]);
            release(outBlock[channel]);
        }
    }
}
