/*
  ==============================================================================

    Distortion.h
    Created: 18 Nov 2014 8:26:32pm
    Author:  milrob

  ==============================================================================
*/

#ifndef DISTORTION_H_INCLUDED
#define DISTORTION_H_INCLUDED

#include "JuceHeader.h"

class DistortionEffect : drow::Buffer::Listener{
public:
    explicit DistortionEffect(drow::Buffer& bufferToControl);
    ~DistortionEffect();
    
    void bufferChanged(drow::Buffer* changedBuffer);
    void resetBuffer();
    void setDistortionAmount(float da);
    
private:
    drow::Buffer& buffer;
    
    void refillBuffer();
    float distortionAmount;
};



#endif  // DISTORTION_H_INCLUDED
