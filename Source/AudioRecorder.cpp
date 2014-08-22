/*
  ==============================================================================

    AudioRecorder.cpp
    Created: 22 Aug 2014 1:09:34pm
    Author:  milrob

  ==============================================================================
*/

#include "JuceHeader.h"

class AudioRecorder : public AudioIODeviceCallback {
public:
    AudioRecorder() :
        backgroundThread("Audio Recorder Thread"),
        sampleRate(0), nextSampleNum(0), activeWriter(nullptr)
    {
        backgroundThread.startThread();
    }
    
    ~AudioRecorder(){ stop(); }
    
    void startRecording(const File& file){
        stop();
        if (sampleRate > 0) {
            file.deleteFile();
            ScopedPointer<FileOutputStream> fileStream(file.createOutputStream());
            if (fileStream != nullptr) {
                WavAudioFormat wavFormat;
                AudioFormatWriter* writer = wavFormat.createWriterFor(fileStream,
                                                                      sampleRate, 1, 16, StringPairArray(), 0);
                if (writer != nullptr) {
                    fileStream.release();
                    threadedWriter = new AudioFormatWriter::ThreadedWriter(writer,
                                                                           backgroundThread, 32768);
                   // thumbnail.reset(writer->getNumChannels(), writer->getSampleRate());
                    nextSampleNum = 0;
                    const ScopedLock sl(writerLock);
                    activeWriter = threadedWriter;
                }
            }
        }
    }
    
    
    void stop(){
        {
            const ScopedLock sl(writerLock);
            activeWriter = nullptr;
        }
        
        threadedWriter = nullptr;
    }
    
    bool isRecording() const { return activeWriter != nullptr; }
    
    void audioDeviceAboutToStart(AudioIODevice* device) override {
        sampleRate = device->getCurrentSampleRate();
    }
    
    void audioDeviceStopped() override { sampleRate = 0; }
    
    void audioDeviceIOCallback(const float** inputChannelData, int, float** outputChannelData,
                               int numOutputChannels, int numSamples) override
    {
        const ScopedLock sl(writerLock);
        if (activeWriter != nullptr) {
            activeWriter->write(inputChannelData, numSamples);
            const AudioSampleBuffer buffer(const_cast<float**>(inputChannelData),
                                           numOutputChannels, numSamples);
          //  thumbnail.addBlock(nextSampleNum, buffer, 0, numSamples);
            nextSampleNum += numSamples;
        }
        
        for (int i = 0; i < numOutputChannels; ++i) {
            if (outputChannelData[i] != nullptr) {
                FloatVectorOperations::clear(outputChannelData[i], numSamples);
            }
        }
    }
    
    
private:
   // AudioThumbnail& thumbnail;
    TimeSliceThread backgroundThread;
    ScopedPointer<AudioFormatWriter::ThreadedWriter> threadedWriter;
    double sampleRate;
    int64 nextSampleNum;
    
    CriticalSection writerLock; //mutex
    AudioFormatWriter::ThreadedWriter* volatile activeWriter;
};
