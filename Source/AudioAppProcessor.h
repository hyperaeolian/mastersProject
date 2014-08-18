/*
  ==============================================================================

    AudioProcessor.h
    Created: 14 Aug 2014 3:43:47pm
    Author:  milrob

  ==============================================================================
*/

#ifndef AUDIOPROCESSOR_H_INCLUDED
#define AUDIOPROCESSOR_H_INCLUDED

#include "AudioApp.h"
#include "LoopGenerator.h"

class AppAudioProcessor : public ChangeBroadcaster, public ChangeListener, public drow::AudioFilePlayer::Listener {
public:
    
    AppAudioProcessor(File& _file);
    AppAudioProcessor(AppAudioProcessor& proc);
    AppAudioProcessor& operator=(AppAudioProcessor&& proc);
    ~AppAudioProcessor();

    AudioDeviceManager       deviceManager;
    AudioSourcePlayer        sourcePlayer;
    drow::AudioFilePlayerExt mediaPlayer;
    
    void initPlayback();
    void gainChanged(ScopedPointer<Slider> gainSlider);
    void changeListenerCallback(ChangeBroadcaster* src) override;
    void shiftyLooping(Loop& currentLoop);
    
    void playerStoppedOrStarted(drow::AudioFilePlayer* player) override;
 
private:
    AppAudioProcessor();
    Loop loop;
   
    float gain;
    File file;
    
    void fileChanged(drow::AudioFilePlayer* player) override ;
    void audioFilePlayerSettingChanged(drow::AudioFilePlayer* player, int settingCode) override;
    

    
};



#endif  // AUDIOPROCESSOR_H_INCLUDED
