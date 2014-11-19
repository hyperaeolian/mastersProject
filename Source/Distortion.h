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

class DistortionEffect : drow::Buffer::Listener, SliderListener {
public:
    DistortionEffect(drow::Buffer& bufferToControl, ScopedPointer<Slider>& _distortionSlider);
    ~DistortionEffect();
    
    void bufferChanged(drow::Buffer* changedBuffer);
    void resetBuffer();
    
    void sliderValueChanged(Slider* slider);
    
private:
    juce::ScopedPointer<Slider> distortionSlider;
    drow::Buffer& buffer;
    
    void refillBuffer(float xn);
    float distortionAmount;
};



#endif  // DISTORTION_H_INCLUDED
