#ifndef __JUCE_HEADER_63FD855A234897E__
#define __JUCE_HEADER_63FD855A234897E__

//[Headers]     -- You can add your own extra header files here --
#include "JuceHeader.h"
#include "LoopGenerator.h"

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
        Looping,
    };

    enum Parameters{
        gainParam = 0,
        delayParam,
        totalNumParams
    };

    float gain, delay;
    juce::Logger* masterLogger;
    bool shifting, forward;
    
    void changeState(TransportState newState);
    void changeListenerCallback(ChangeBroadcaster* src);
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
    //[UserVariables]   
    const int APP_WIDTH = 700, APP_HEIGHT = 750;
    AudioDeviceManager       deviceManager;
    AudioSourcePlayer        sourcePlayer;
    drow::AudioFilePlayerExt mediaPlayer;
    drow::AudioFilePlayer::Listener* listener;
    
    TransportState state;
    vector<Loop> _crudeLoops;
    std::string AUDIO_FILENAME;
    Loop* currentLoop;
    
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
