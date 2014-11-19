/*
  ==============================================================================

    BufferTransform.h
    Created: 18 Nov 2014 8:26:18pm
    Author:  milrob

  ==============================================================================
*/

#ifndef BUFFERTRANSFORM_H_INCLUDED
#define BUFFERTRANSFORM_H_INCLUDED

#include "JuceHeader.h"

class BufferTransform :  public AudioSource
{
public:
    //==============================================================================
    BufferTransform (AudioSource* source,
                                bool deleteSourceWhenDeleted = false);
    
    /** Destructor. */
    ~BufferTransform();
    
    /** Setting this to true does not apply the buffer. */
    void setBypass (bool shouldBypass);
    
    /** Returns all of the settings.
     */
    drow::Buffer& getBuffer()     {   return buffer;    }
    
    //==============================================================================
    /** @internal. */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate);
    
    /** @internal. */
    void releaseResources();
    
    /** @internal. */
    void getNextAudioBlock (const AudioSourceChannelInfo& info);
    
private:
    //==============================================================================
    OptionalScopedPointer<AudioSource> source;
    drow::Buffer buffer;
    bool isBypassed;
    
    //==============================================================================

};




#endif  // BUFFERTRANSFORM_H_INCLUDED
