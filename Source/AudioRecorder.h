/*
  ==============================================================================

    AudioRecorder.h
    Created: 29 Aug 2014 12:23:36pm
    Author:  milrob

  ==============================================================================
*/

#ifndef AUDIORECORDER_H_INCLUDED
#define AUDIORECORDER_H_INCLUDED

#include "JuceHeader.h"

class AudioRecorder : public AudioIODeviceCallback {
public:
    AudioRecorder();
    ~AudioRecorder();
    
    void startRecording(const File& file);
    void stop();
    
    bool isRecording() const { return activeWriter != nullptr; }
    
    void audioDeviceAboutToStart(AudioIODevice* device) override {
        sampleRate = device->getCurrentSampleRate();
    }
    
    void audioDeviceStopped() override { sampleRate = 0; }
    
    void audioDeviceIOCallback(const float** inputChannelData, int, float** outputChannelData,
                               int numOutputChannels, int numSamples) override;
    
    
private:
    TimeSliceThread backgroundThread;
    ScopedPointer<AudioFormatWriter::ThreadedWriter> threadedWriter;
    double sampleRate;
    int64 nextSampleNum;
    
    CriticalSection writerLock; //mutex
    AudioFormatWriter::ThreadedWriter* volatile activeWriter;
};



#endif  // AUDIORECORDER_H_INCLUDED
