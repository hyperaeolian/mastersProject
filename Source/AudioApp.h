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
#include "LoopGenerator.h"
#include "MarkovChain.h"
#include "MATRIX.h"
#include "ShiftyLooping.h"
#include "Design.h"
#include "AudioWaveform.h"
#include "AudioRecorder.h"
#include "ProgressWindow.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
 TO DO:
    Restructure program: use these separate components for the synthesis, analysis,
    and GUI

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
        Recording,
        Looping,
        ShiftyLooping
    };

    
    //State Methods
    void changeState(TransportState newState);
    void printCurrentState(String s);

    //File Player Methods
    void changeListenerCallback(ChangeBroadcaster* src);
    void fileChanged(drow::AudioFilePlayer* player) override;
    void audioFilePlayerSettingChanged(drow::AudioFilePlayer* player, int settingCode) override;
    void timerCallback() override;
    void playerStoppedOrStarted(drow::AudioFilePlayer* player) override;

    //Recording
    void startRecording();
    void stopRecording();

    //Utility Methods
    void loadFile();
    void initialize();
    void openAudioSettings();
    void showLoopTable();
    bool isTableEnabled(){ return tableEnabled; }
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void buttonClicked (Button* buttonThatWasClicked);
    void sliderValueChanged (Slider* sliderThatWasMoved);

    // Binary resources:
    static const char* knob_png;
    static const int knob_pngSize;
    static const char* background_png;
    static const int background_pngSize;


private:
    //[UserVariables]   -- You can add your own custom variables in this section.

    
    
    essentia::Real Tempo;
    File* auxFile;
    bool tableEnabled;
    
    //Audio Device Vars
    AudioDeviceManager  deviceManager;
    AudioSourcePlayer   sourcePlayer;
    AudioRecorder       recorder;
    drow::AudioFilePlayerExt mediaPlayer;
    ShiftyLooper shiftyLooper;
    
    //State & Loop Vars
    TransportState state;
    Loop* currentLoop;
    std::vector<Loop> crudeLoops;
    bool shifting, forward;
    std::string audiofilename;

    //Distance and Markov Vars
    const int MarkovIterations;
    MATRIX* similarity;
    juce::ScopedPointer<MATRIX> transMat;
    std::vector<int> markov_chain;

    //Utility Vars
    float gain;
    juce::Random random;
    juce::Logger* masterLogger;
    MemoryInputStream stream;


    //Views
    CustomLookAndFeel* design;
    ScopedPointer<Waveform> waveform;
    ScopedPointer<ImageComponent> backgroundImage;
    
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> infoLabel;
    ScopedPointer<TextButton> playButton;
    ScopedPointer<TextButton> stopButton;
    ScopedPointer<TextButton> recordingButton;
    ScopedPointer<ToggleButton> loopButton;
    ScopedPointer<Slider> gainSlider;
    ScopedPointer<Label> gainLabel;
    ScopedPointer<ToggleButton> shiftyLoopingButton;
    ScopedPointer<GroupComponent> ostinatoGroup;
    ScopedPointer<GroupComponent> pitchTempoGropu;
    ScopedPointer<Slider> rateSlider;
    ScopedPointer<Slider> pitchSlider;
    ScopedPointer<Slider> tempoSlider;
    ScopedPointer<Slider> varianceSlider;
    ScopedPointer<Slider> barSizeSlider;
    ScopedPointer<TextButton> reloopButton;
    ScopedPointer<Label> varianceLabel;
    ScopedPointer<Label> barSizeLabel;
    ScopedPointer<Label> rateLabel;
    ScopedPointer<Label> pitchLabel;
    ScopedPointer<Label> tempoLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioApp)
};

//[EndFile] You can add extra defines here...
#define _REAL essentia::Real


//[/EndFile]

#endif   // __JUCE_HEADER_63FD855A234897E__
