/*
  ==============================================================================

    AudioProcessor.cpp
    Created: 14 Aug 2014 3:43:47pm
    Author:  milrob

  ==============================================================================
*/

#include "AudioAppProcessor.h"

AppAudioProcessor::AppAudioProcessor(File& _file) : file(_file) {
    deviceManager.initialise(0, 2, nullptr, true);
    deviceManager.addAudioCallback(&sourcePlayer);
    sourcePlayer.setSource(&mediaPlayer);
    deviceManager.addChangeListener(this);
    mediaPlayer.addListener(this);
    mediaPlayer.setFile(file);
    
}

AppAudioProcessor::~AppAudioProcessor(){
    if (mediaPlayer.hasStreamFinished()) mediaPlayer.removeListener(this);
    deviceManager.removeAudioCallback(&sourcePlayer);
}

void AppAudioProcessor::gainChanged(ScopedPointer<Slider> gainSlider){
    gain = static_cast<float>(gainSlider->getValue());
    sourcePlayer.setGain(gain);
}

void AppAudioProcessor::changeListenerCallback(juce::ChangeBroadcaster *src){
        
        if (&deviceManager == src) {
            AudioDeviceManager::AudioDeviceSetup setup;
            deviceManager.getAudioDeviceSetup(setup);
            setup.outputChannels.isZero() ? sourcePlayer.setSource(nullptr)
            : sourcePlayer.setSource(&mediaPlayer);
        }
        

}

void AppAudioProcessor::shiftyLooping(Loop& currentLoop){
    if (mediaPlayer.hasStreamFinished()) {
        mediaPlayer.stop();
        mediaPlayer.setLoopTimes(currentLoop.start, currentLoop.end);
        mediaPlayer.setPosition(currentLoop.start);
        // mediaPlayer.setLoopBetweenTimes(shiftyLoopingButton->getToggleState());
        mediaPlayer.start();
    }
}

void AppAudioProcessor::fileChanged(drow::AudioFilePlayer* player){}
void AppAudioProcessor::audioFilePlayerSettingChanged(drow::AudioFilePlayer* player, int settingCode){}
void AppAudioProcessor::playerStoppedOrStarted(drow::AudioFilePlayer* player){}