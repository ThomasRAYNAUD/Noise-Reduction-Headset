#include "MyDsp.h"
#define NUM_COEFFICIENTS 128
#define AUDIO_OUTPUTS 2
#define MULT_16 32767
#define DIV_16 0.0000305185
#define BUFFER_SIZE NUM_COEFFICIENTS


MyDsp::MyDsp() : AudioStream(AUDIO_OUTPUTS, new audio_block_t*[AUDIO_OUTPUTS]),
noise(),
oneZero(),
mu(0.0001),
signal_de_reference(0)
{
  for (int i = 0; i < NUM_COEFFICIENTS; i++) { // remplir le tab de coef
    coefficients[i] = 0 ;
    input[i] = 0 ;
  }
}

MyDsp::~MyDsp(){}

void MyDsp::entree(float sample) {
    for (int j = NUM_COEFFICIENTS - 1; j > 0; j--) {
            input[j] = input[j - 1];
    }
    input[0] = sample;
}

float MyDsp::sortie() {
    float signal_sortie = 0;
    for (int j = 0; j < NUM_COEFFICIENTS; j++) {
        signal_sortie += coefficients[j] * input[0];
    }
    return signal_sortie;
}

float MyDsp::erreur(float sample, float signal_sortie) {
    float error = sample - signal_sortie;
    return error;
}

void MyDsp::coef(float erreur) {
    for (int k = 0; k < NUM_COEFFICIENTS; k++) {
        coefficients[k] += mu * erreur * input[0];
    }
}

void MyDsp::update(void) {
  audio_block_t* outBlock[AUDIO_OUTPUTS];
  for (int channel = 0; channel < AUDIO_OUTPUTS; channel++) {
    outBlock[channel] = allocate();
    if (outBlock[channel]) {
        
      
      for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        float currentSample = oneZero.tick(noise.tick())*0.5 ;
        entree(currentSample);
        float signal_sortie = sortie() ;
        float error = erreur(currentSample, signal_sortie) ;
        Serial.println(error);
        coef(error);
        
        signal_sortie = max(-1.0, min(1.0, signal_sortie)); 
        int16_t val = signal_sortie * MULT_16;
        outBlock[channel]->data[i] = val;
      }
      transmit(outBlock[channel], channel);
      release(outBlock[channel]);
    }
  }
}
