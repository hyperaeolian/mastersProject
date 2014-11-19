/*
  ==============================================================================

    Distortion.cpp
    Created: 18 Nov 2014 8:26:32pm
    Author:  milrob

  ==============================================================================
*/

#include "Distortion.h"

DistortionEffect::DistortionEffect(drow::Buffer& bufferToControl, ScopedPointer<Slider>& _distortionSlider)
: distortionSlider(_distortionSlider), buffer(bufferToControl)
{
    buffer.addListener(this);
    distortionAmount = 0.0f;
}

DistortionEffect::~DistortionEffect(){
    buffer.removeListener(this);
}

void DistortionEffect::bufferChanged(drow::Buffer* changedBuffer){
    if (changedBuffer == &buffer) {
        
    }
}

void DistortionEffect::sliderValueChanged(juce::Slider *slider){
    if (slider == distortionSlider){
        distortionAmount = slider->getValue();
    }
}

void DistortionEffect::refillBuffer(float xn){
    float* bufferData = buffer.getData();
    const int bufferSize = buffer.getSize();
    const float bufferScale = 1.0f / float(bufferSize);
    
    for (int i = 0; i < bufferSize; ++i) {
        float x = jlimit(0.0f, 1.0f, i * bufferScale + distortionAmount);
        bufferData[i] = x;
    }
    
    buffer.updateListeners();
    
}


void DistortionEffect::resetBuffer(){
    float* bufferData = buffer.getData();
    const int bufferSize = buffer.getSize();
    const float bufferScale = 1.0f / bufferSize;
    
	for (int i = 0; i < bufferSize; ++i)
        bufferData[i] = bufferScale * i;
    
    buffer.updateListeners();
}