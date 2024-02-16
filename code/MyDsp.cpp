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
    coefficients[i] = random(1000) /1000 - 0.5 ;
    buffer[i] = 0;
  }
}

MyDsp::~MyDsp(){}

void MyDsp::update(void) {
  audio_block_t* outBlock[AUDIO_OUTPUTS];
  for (int channel = 0; channel < AUDIO_OUTPUTS; channel++) {
    outBlock[channel] = allocate();
    if (outBlock[channel]) {

      
      for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
        float currentSample = oneZero.tick(noise.tick())*0.5;
        
        signal_de_reference = currentSample;
        float signal_sortie = 0;

        for (int j = 0; j < NUM_COEFFICIENTS; j++) {
             signal_sortie += coefficients[j] * buffer[j];
        }
        float erreur = signal_de_reference - signal_sortie;
        for (int k = 0; k < NUM_COEFFICIENTS; k++) {
            coefficients[k] += mu * erreur * buffer[k];
        }
        for (int j = NUM_COEFFICIENTS - 1; j > 0; j--) {
            buffer[j] = buffer[j - 1];
        }
        buffer[0] = currentSample;

        signal_sortie = max(-1.0, min(1.0, signal_sortie)); 
        int16_t val = signal_sortie * MULT_16;
        outBlock[channel]->data[i] = val;
      }
      transmit(outBlock[channel], channel);
      release(outBlock[channel]);
    }
  }
}
