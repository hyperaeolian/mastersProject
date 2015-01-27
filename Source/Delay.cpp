/*
  ==============================================================================

    Delay.cpp
    Created: 18 Nov 2014 7:41:54pm
    Author:  milrob

  ==============================================================================
*/

#include "Delay.h"

DelayEffect::DelayEffect(float _time, float _decay) : delayBuffer(2,12000) {
  
    delayPosition = 0;
}

DelayEffect::~DelayEffect(){
    
}

void DelayEffect::prepareToPlay(double sampleRate, int samplesPerBlock){
    
}

void DelayEffect::releaseResources(){
    
}

void DelayEffect::processBlock(juce::AudioSampleBuffer& buffer){
    buffer.clear();
}










