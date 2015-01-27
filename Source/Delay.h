/*
  ==============================================================================

    Delay.h
    Created: 18 Nov 2014 7:41:54pm
    Author:  milrob

  ==============================================================================
*/

#ifndef DELAY_H_INCLUDED
#define DELAY_H_INCLUDED

#include "JuceHeader.h"

class DelayEffect : public AudioAppComponent {
public:
    DelayEffect(float _time, float _decay);
    ~DelayEffect();
    
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources() override;
    void processBlock (AudioSampleBuffer& buffer);
   

//==============================================================================
   

private:
//==============================================================================
    AudioSampleBuffer delayBuffer;
    int delayPosition;
    int size;
    float decay;

};


#endif  // DELAY_H_INCLUDED
