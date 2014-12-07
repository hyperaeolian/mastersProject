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

class DelayEffect : public AudioProcessor {
public:
    DelayEffect();
    ~DelayEffect();
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;
    void reset() override;

//==============================================================================
    bool hasEditor() const override { return false; }
    AudioProcessorEditor* createEditor() override {}

    const String getName() const override { return "Effect"; }

    int   getNumParameters() override;
    float getParameter             (int index) override;
    float getParameterDefaultValue (int index) override;
    void  setParameter             (int index, float newValue) override;
    const String getParameterName  (int index) override;
    const String getParameterText  (int index) override;

    const String getInputChannelName  (int channelIndex) const override;
    const String getOutputChannelName (int channelIndex) const override;
    bool isInputChannelStereoPair     (int index) const override;
    bool isOutputChannelStereoPair    (int index) const override;

    bool acceptsMidi() const override {return false;}
    bool producesMidi() const override {return false;}
    bool silenceInProducesSilenceOut() const override {return false;}
    double getTailLengthSeconds() const override {return 0.0f;}


    int getNumPrograms() override                         { return 1; }
    int getCurrentProgram() override                      { return 0; }
    void setCurrentProgram (int /*index*/) override       {}
    const String getProgramName (int /*index*/) override  { return "Default"; }
    void changeProgramName (int /*index*/, const String& /*newName*/) override  {}

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override {}
    void setStateInformation (const void* data, int sizeInBytes) override {}

    //==============================================================================
    // These properties are public so that our editor component can access them
    // A bit of a hacky way to do it, but it's only a demo! Obviously in your own
    // code you'll do this much more neatly..

    // this is kept up to date with the midi messages that arrive, and the UI component
    // registers with it so it can represent the incoming messages
    //MidiKeyboardState keyboardState;

    // this keeps a copy of the last set of time info that was acquired during an audio
    // callback - the UI component will read this and display it.
    //AudioPlayHead::CurrentPositionInfo lastPosInfo;

    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
   // int lastUIWidth, lastUIHeight;

//==============================================================================
    enum Parameters {
        gainParam = 0,
        delayParam,
        totalNumParams
    };

    float gain, delay;

private:
//==============================================================================
    AudioSampleBuffer delayBuffer;
    int delayPosition;

    // the synth!
   // Synthesiser synth;

};



//class DelayEffect : public drow::Buffer::Listener {
//public:
//    DelayEffect();
//    ~DelayEffect();
//
//
//    float delay;
//
//private:
//    AudioSampleBuffer delayBuffer;
//    int delayPosition;
//};


#endif  // DELAY_H_INCLUDED
