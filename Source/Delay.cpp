/*
  ==============================================================================

    Delay.cpp
    Created: 18 Nov 2014 7:41:54pm
    Author:  milrob

  ==============================================================================
*/

#include "Delay.h"

DelayEffect::DelayEffect() : delayBuffer(2,12000) {
    gain = 0.0f;
    delay = 0.0f;
    
    //lastUIWidth = 400;
    //lastUIHeight = 200;
    
    //lastPosInfo.resetToDefault();
    delayPosition = 0;
}

DelayEffect::~DelayEffect(){
    
}

inline int DelayEffect::getNumParameters(){ return totalNumParams; }

float DelayEffect::getParameter(int index){
    switch (index){
        case gainParam:     return gain;
        case delayParam:    return delay;
        default:            return 0.0f;
    }
}


float DelayEffect::getParameterDefaultValue (int index){
    switch (index)
    {
        case gainParam:     return 0.0f;
        case delayParam:    return 0.0f;
        default:            break;
    }
    
    return 0.0f;
}
void  DelayEffect::setParameter(int index, float newValue) {}


const String DelayEffect::getParameterName  (int index) { switch (index)
    {
        case gainParam:     return "gain";
        case delayParam:    return "delay";
        default:            break;
    }
    
    return String::empty;}


const String DelayEffect::getParameterText  (int index) {
    return String (getParameter (index), 2);
}

void DelayEffect::prepareToPlay (double sampleRate, int samplesPerBlock) {
    delayBuffer.clear();
}

void DelayEffect::releaseResources(){}


void DelayEffect::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) {
    const int numSamples = buffer.getNumSamples();
    int delay_pos = 0;
    
    for (int c = 0; c < getNumInputChannels(); ++c) {
        //buffer.applyGain(c, 0, numSamples, 0.0);
        ScopedPointer<float> channelData = buffer.getWritePointer(c);
        ScopedPointer<float> delayData = delayBuffer.getWritePointer(jmin(c, delayBuffer.getNumChannels()-1));
        delay_pos = delayPosition;
        
        for (int i = 0; i < numSamples; ++i) {
            const float in = channelData[i];
            channelData[i] += delayData[delay_pos];
            delayData[delay_pos] = (delayData[delay_pos] + in) * delay;
            if (++delay_pos >= delayBuffer.getNumSamples()) {
                delay_pos = 0;
            }
        }
        delayPosition = delay_pos;
        
        for (int i = 0; i < getNumInputChannels(); ++i) {
            buffer.clear(i, 0, buffer.getNumSamples());
        }
//        AudioPlayHead::CurrentPositionInfo newTime;
//        if (getPlayHead() != nullptr && getPlayHead()->getCurrentPosition(newTime)) {
//            lastPosInfo = newTime;
//        } else {
//            lastPosInfo.resetToDefault();
//        }
    }
    
    
}
void DelayEffect::reset() { delayBuffer.clear();}

bool DelayEffect::isInputChannelStereoPair(int) const { return true; }
bool DelayEffect::isOutputChannelStereoPair(int) const { return true; }

const String DelayEffect::getInputChannelName (const int channelIndex) const {
    return String(channelIndex + 1);
}

const String DelayEffect::getOutputChannelName (const int channelIndex) const {
    return String(channelIndex + 1);
}
















