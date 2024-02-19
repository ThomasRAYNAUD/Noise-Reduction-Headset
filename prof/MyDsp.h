#ifndef faust_teensy_h_
#define faust_teensy_h_
#define NUM_COEFFICIENTS 128
#define AUDIO_OUTPUTS 2

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
    void LMS(float sample, int i, int channel, audio_block_t** outBlock , audio_block_t** inBlock);
    void pushInput(float sample);
    void outputSys(int i, int channel,audio_block_t** outBlock , audio_block_t** inBlock);
    void adaptativeFilter();
    void outputError();
    void correctionCoef();

  private:
    Noise noise;
    OneZero oneZero;
    float mu ; 
    float adaptSignal;
    float error ;
    float output_H ;
    float input[NUM_COEFFICIENTS] ;
    float coefficients[NUM_COEFFICIENTS];
};

#endif
