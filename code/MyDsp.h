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
    void setB1(float b1);
    
  private:
    Noise noise;
    OneZero oneZero;
    float32_t coefficients[NUM_COEFFICIENTS];
    float mu ; 
    float signal_de_reference ;
    float buffer[NUM_COEFFICIENTS];
};

#endif
