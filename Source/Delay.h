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

class DelayEffect : public drow::Buffer::Listener {
public:
    DelayEffect();
    ~DelayEffect();
    

    float delay;
    
private:
    AudioSampleBuffer delayBuffer;
    int delayPosition;
};


#endif  // DELAY_H_INCLUDED
