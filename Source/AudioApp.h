/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.0

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

#ifndef __JUCE_HEADER_63FD855A234897E__
#define __JUCE_HEADER_63FD855A234897E__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "LoopGen.h"

//[/Headers]

//class MediaPlayerListener;

//==============================================================================
/**
                                                                    //[Comments]

                                                                    //[/Comments]
*/
class AudioApp  : public Component,
                  public ChangeListener,
                  public ButtonListener,
                  public SliderListener,
                  public Timer,
                  public drow::AudioFilePlayer::Listener

{
public:
    //==============================================================================
    AudioApp ();
    ~AudioApp();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    enum TransportState{
        Stopped,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping,
        Looping,
    };

    enum Parameters{
        gainParam = 0,
        delayParam,
        totalNumParams
    };

    float gain, delay;
    bool shifting, forward;
    void changeState(TransportState newState);
    void changeListenerCallback(ChangeBroadcaster* src);
    static std::vector<float> ONSETS;
    void shiftyLooping();
    void printCurrentState(String s);
    
    void fileChanged(drow::AudioFilePlayer* player) override;
    void audioFilePlayerSettingChanged(drow::AudioFilePlayer* player, int settingCode) override;
    void timerCallback();
    void playerStoppedOrStarted(drow::AudioFilePlayer* player) override;
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    const int APP_WIDTH = 700, APP_HEIGHT = 750;
    AudioDeviceManager       deviceManager;
    AudioSourcePlayer        sourcePlayer;
    drow::AudioFilePlayerExt mediaPlayer;
    drow::AudioFilePlayer::Listener* listener;
    
    TransportState state;
    vector<Loop> _crudeLoops;
    std::string AUDIO_FILENAME;
    Loop* currentLoop;
    juce::Logger* masterLogger;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> infoLabel;
    ScopedPointer<Label> appLabel;
    ScopedPointer<GroupComponent> mainGroup;
    ScopedPointer<TextButton> loadButton;
    ScopedPointer<TextButton> playButton;
    ScopedPointer<TextButton> stopButton;
    ScopedPointer<TextButton> settingsButton;
    ScopedPointer<ToggleButton> loopButton;
    ScopedPointer<Slider> gainSlider;
    ScopedPointer<Label> gainLabel;
    ScopedPointer<ToggleButton> shiftyLoopingButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioApp)
};

//[EndFile] You can add extra defines here...

//[/EndFile]

#endif   // __JUCE_HEADER_63FD855A234897E__
