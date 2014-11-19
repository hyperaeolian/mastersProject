/*
  ==============================================================================

    BufferTransform.cpp
    Created: 18 Nov 2014 8:26:18pm
    Author:  milrob

  ==============================================================================
*/

#include "BufferTransform.h"

BufferTransform::BufferTransform(AudioSource* source_, bool deleteSourceWhenDeleted)
    : source (source_, deleteSourceWhenDeleted), buffer (512)
{
    jassert (source_ != nullptr);
    
    const float xScale = 1.0f / (buffer.getSize() - 1);
    
    for (int i = 0; i < buffer.getSize(); ++i)
        buffer.getReference (i) = i * xScale;
    
}

BufferTransform::~BufferTransform()
{
}

void BufferTransform::setBypass (bool shouldBypass)
{
    isBypassed = shouldBypass;
}

void BufferTransform::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    source->prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void BufferTransform::releaseResources()
{
    source->releaseResources();
}

void BufferTransform::getNextAudioBlock (const AudioSourceChannelInfo& info)
{
    source->getNextAudioBlock (info);
    
    if (! isBypassed)
    {
        const int bufferSize = buffer.getSize() - 1;
        float** channelData = info.buffer->getArrayOfWritePointers();
        float sample;
        
        for (int c = 0; c < info.buffer->getNumChannels(); ++c){
            for (int s = 0; s < info.numSamples; ++s){
                sample = channelData[c][s];
                
                if (sample < 0.0f && sample >= -1.0f){
                    sample *= -1.0f;
                    sample = drow::linearInterpolate(buffer.getData(), bufferSize, sample * bufferSize);
                    sample *= -1.0f;
                }
                else if (sample >= 0.0f && sample <= 1.0f){
                    sample = drow::linearInterpolate (buffer.getData(), bufferSize, sample * bufferSize);
                }
                
                channelData[c][s] = sample;
            }
        }
    }
}
