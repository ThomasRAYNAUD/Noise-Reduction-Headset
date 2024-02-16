#ifndef faust_teensy_h_
#define faust_teensy_h_
#define NUM_COEFFICIENTS 128

#include "Arduino.h"
#include "AudioStream.h"
#include "Audio.h"

#include "Noise.h"
#include "OneZero.h"

class MyDsp : public AudioStream
{
  public:
    MyDsp();
    ~MyDsp();
    
    virtual void update(void);
    void entree(float sample);
    float sortie();
    float erreur(float sample, float signal_sortie);
    void coef(float erreur);
    
  private:
    Noise noise;
    OneZero oneZero;
    float32_t coefficients[NUM_COEFFICIENTS];
    float mu ; 
    float signal_de_reference ;
    float input[NUM_COEFFICIENTS];
    float error = 0;
    float normalisation = 0;
};

#endif
