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
                signal_de_reference(0),
                error(0),
                normalisation(0)
{
    for (int i = 0; i < NUM_COEFFICIENTS; i++) { // remplir le tableau de coefficients
        coefficients[i] = random(1000) / 1000.0 - 0.5;
        input[i] = 0;
    }
}

MyDsp::~MyDsp() {}

void MyDsp::pushInput(float sample) { // fonction de balayage circulaire > affecter les samples au tab
    for (int i = 0; i < (NUM_COEFFICIENTS - 1); i++) {
        input[i] = input[i + 1];
    }
    input[NUM_COEFFICIENTS - 1] = sample;
}

void MyDsp::coef() { 
    /* mettre à jour tous les coefficients pour tous les échantillons de la voix */
    erreur();
    for (int k = 0; k < NUM_COEFFICIENTS; k++) {
        norm();
        coefficients[k] = coefficients[k] + (mu * error * input[NUM_COEFFICIENTS-1]) / normalisation;
    }
}

void MyDsp::norm() {
    /* faire une normalisation pour le calcul du coefficient */
    normalisation = 0;
    for (int k = 0; k < NUM_COEFFICIENTS; k++) {
        normalisation += input[k] * input[k];
    }
}

void MyDsp::erreur() {
    error = 0;
    for (int v = 0; v < NUM_COEFFICIENTS; v++) {
        float sortie_h = input[NUM_COEFFICIENTS - 1] * coefficients[v];
        error += input[NUM_COEFFICIENTS - 1] - sortie_h ;
        Serial.println(sortie_h);
    }
}

void MyDsp::update(void) {
    audio_block_t *outBlock[AUDIO_OUTPUTS];
    for (int channel = 0; channel < AUDIO_OUTPUTS; channel++) {
        outBlock[channel] = allocate();
        if (outBlock[channel]) {
            for (int i = 0; i < AUDIO_BLOCK_SAMPLES; i++) { // AUDIO_BLOCK_SAMPLES représente la taille du buffer x[n]
                float currentSample = oneZero.tick(noise.tick()) * 0.5;
                pushInput(currentSample);
                coef();
                currentSample = max(-1.0, min(1.0, currentSample));
                int16_t val = currentSample * MULT_16;
                outBlock[channel]->data[i] = val;
            }
            transmit(outBlock[channel], channel);
            release(outBlock[channel]);
        }
    }
}
