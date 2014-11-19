/*
  ==============================================================================

    Delay.cpp
    Created: 18 Nov 2014 7:41:54pm
    Author:  milrob

  ==============================================================================
*/

#include "Delay.h"
/*
DelayEffect::DelayEffect() : defaultDelay(0.5f), delayBuffer(2,12000){
    delay = defaultDelay;
    delayPosition = 0;
}


void DelayEffect::prepareToPlay(double sampleRate, int){
    delayBuffer.clear();
}

void DelayEffect::releaseResources(){}

void DelayEffect::reset(){
    delayBuffer.clear();
}

void DelayEffect::processBlock(juce::AudioSampleBuffer &buffer, juce::MidiBuffer &midiMessages){
    const int numSamples = buffer.getNumSamples();
    
    for (int chnl = 0; chnl < getNumInputChannels(); ++chnl){
        float* channelData = buffer.getWritePointer(chnl);
        float* delayData = delayBuffer.getWritePointer(jmin(chnl, delayBuffer.getNumChannels()-1));
        int dp  = delayPosition;
        
        for (int i = 0; i < numSamples; ++i){
            const float in = channelData[i];
            channelData[i] += delayData[dp];
            delayData[dp] = (delayData[dp] + in) * delay;
            if (++dp >= delayBuffer.getNumSamples()) {
                dp = 0;
            }
        }
        
        delayPosition = dp;
        
        for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
            buffer.clear (i, 0, buffer.getNumSamples());
        
        AudioPlayHead::CurrentPositionInfo newTime;
        
        if (getPlayHead() != nullptr && getPlayHead()->getCurrentPosition (newTime)){
            lastPosInfo = newTime;
        } else {
            lastPosInfo.resetToDefault();
        }
    }
}*/