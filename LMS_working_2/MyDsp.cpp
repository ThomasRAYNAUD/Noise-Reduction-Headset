#include "MyDsp.h"
#define AUDIO_OUTPUTS 2
#define MULT_16 32767
#define DIV_16 0.0000305185
#define BUFFER_SIZE NUM_COEFFICIENTS

MyDsp::MyDsp() : AudioStream(AUDIO_OUTPUTS, new audio_block_t*[AUDIO_OUTPUTS]),
                noise(),
                oneZero(),
                mu(0.01),
                adaptSignal(0.0),
                error(0),
                output_H(0)
{
    for (int i = 0; i < NUM_COEFFICIENTS; i++) {
        coefficients[i] = 0.0 ;
        input[i] = 0.0 ;
    }
}

MyDsp::~MyDsp() {}

void MyDsp::LMS(float sample) {
  pushInput(sample);
  outputSys();
  adaptativeFilter();
  outputError();
  correctionCoef();
}

void MyDsp::pushInput(float sample) {
    for (int i = NUM_COEFFICIENTS - 1; i > 0; i--) {
        input[i] = input[i - 1];
    }
    input[0] = sample;
}

void MyDsp::outputSys() {
    output_H = input[0] * 0.44 ;
}

void MyDsp::adaptativeFilter() {
  adaptSignal = 0 ;
  for (int v = 0; v < NUM_COEFFICIENTS; v++) {
      adaptSignal += input[v] * coefficients[v];
  }
}

void MyDsp::outputError() {
  error = output_H - adaptSignal ;
  Serial.println(error);
}

void MyDsp::correctionCoef() {
  float norm = 0;
  for (int j = 0 ; j < NUM_COEFFICIENTS ; j++){
    norm += input[j]*input[j];
  }
  for (int k = 0; k < NUM_COEFFICIENTS; k++) {
      coefficients[k] += (mu * error * input[k])/norm ;
  }
}

void MyDsp::update(void) {
    audio_block_t *outBlock[AUDIO_OUTPUTS];
    for (int channel = 0; channel < AUDIO_OUTPUTS; channel++) {
        outBlock[channel] = allocate();
        if (outBlock[channel]) {
            for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
                float currentSample = oneZero.tick(noise.tick()) * 0.5;
                LMS(currentSample);
                currentSample = max(-1.0, min(1.0, error));
                int16_t val = currentSample * MULT_16;
                outBlock[channel]->data[i] = val;
            }
            transmit(outBlock[channel], channel);
            release(outBlock[channel]);
        }
    }
}
