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

//[Headers] You can add your own extra header files here...
#include "MainComponent.h"
//[/Headers]

#include "AudioApp.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
AudioApp::AudioApp () : waveform(mediaPlayer)
{
    addAndMakeVisible (infoLabel = new Label ("Info Label",
                                              TRANS("Data")));
    infoLabel->setFont (Font ("Apple LiSung", 17.90f, Font::plain));
    infoLabel->setJustificationType (Justification::centredLeft);
    infoLabel->setEditable (false, false, false);
    infoLabel->setColour (Label::backgroundColourId, Colour (0xff0d0d0d));
    infoLabel->setColour (Label::textColourId, Colour (0xff87ee20));
    infoLabel->setColour (TextEditor::textColourId, Colours::black);
    infoLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (appLabel = new Label ("App Name Label",
                                             TRANS("SL480x")));
    appLabel->setFont (Font ("Arial Black", 34.40f, Font::plain));
    appLabel->setJustificationType (Justification::centredLeft);
    appLabel->setEditable (false, false, false);
    appLabel->setColour (Label::backgroundColourId, Colour (0x00dfd3d3));
    appLabel->setColour (Label::textColourId, Colour (0xfffdfdfb));
    appLabel->setColour (TextEditor::textColourId, Colours::black);
    appLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (loadButton = new TextButton ("Load Button"));
    loadButton->setButtonText (TRANS("Load File..."));
    loadButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    loadButton->addListener (this);
    loadButton->setColour (TextButton::buttonColourId, Colours::aliceblue);

    addAndMakeVisible (playButton = new TextButton ("Play Button"));
    playButton->setButtonText (TRANS("Play"));
    playButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    playButton->addListener (this);
    playButton->setColour (TextButton::buttonColourId, Colours::aliceblue);

    addAndMakeVisible (stopButton = new TextButton ("Stop Button"));
    stopButton->setButtonText (TRANS("Stop"));
    stopButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    stopButton->addListener (this);
    stopButton->setColour (TextButton::buttonColourId, Colours::aliceblue);

    addAndMakeVisible (settingsButton = new TextButton ("Settings Button"));
    settingsButton->setButtonText (TRANS("Settings..."));
    settingsButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight);
    settingsButton->addListener (this);
    settingsButton->setColour (TextButton::buttonColourId, Colours::aliceblue);

    addAndMakeVisible (loopButton = new ToggleButton ("Loop Button"));
    loopButton->setButtonText (TRANS("Loop Sample"));
    loopButton->setConnectedEdges (Button::ConnectedOnLeft | Button::ConnectedOnRight | Button::ConnectedOnTop | Button::ConnectedOnBottom);
    loopButton->addListener (this);
    loopButton->setColour (ToggleButton::textColourId, Colour (0xffe8d8d8));

    addAndMakeVisible (gainSlider = new Slider ("Gain Slider"));
    gainSlider->setRange (0, 1, 0.1);
    gainSlider->setSliderStyle (Slider::LinearVertical);
    gainSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    gainSlider->setColour (Slider::thumbColourId, Colours::aquamarine);
    gainSlider->setColour (Slider::trackColourId, Colours::aquamarine);
    gainSlider->setColour (Slider::rotarySliderOutlineColourId, Colours::blue);
    gainSlider->addListener (this);
    gainSlider->setSkewFactor (2);

    addAndMakeVisible (gainLabel = new Label ("Gain Label",
                                              TRANS("GAIN")));
    gainLabel->setFont (Font ("Arial Black", 22.30f, Font::plain));
    gainLabel->setJustificationType (Justification::centred);
    gainLabel->setEditable (false, false, false);
    gainLabel->setColour (Label::textColourId, Colours::azure);
    gainLabel->setColour (TextEditor::textColourId, Colours::black);
    gainLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    gainLabel->setColour (TextEditor::highlightColourId, Colours::red);

    addAndMakeVisible (shiftyLoopingButton = new ToggleButton ("Shifty Button"));
    shiftyLoopingButton->setButtonText (TRANS("Shifty Looping"));
    shiftyLoopingButton->addListener (this);
    shiftyLoopingButton->setColour (ToggleButton::textColourId, Colour (0xfff3f3f3));


    //[UserPreSize]
    setSize(APP_WIDTH, APP_HEIGHT);
    //[/UserPreSize]


    //[Constructor] You can add your own custom stuff here..
    startTimer(200);
    playButton->setEnabled(false);
    stopButton->setEnabled(false);
    shiftyLoopingButton->setEnabled(false);
    shiftyLoopingButton->setClickingTogglesState(true);
    loopButton->setEnabled(false);

    deviceManager.initialise(0, 2, nullptr, true);
    deviceManager.addAudioCallback(&sourcePlayer);
    sourcePlayer.setSource(&mediaPlayer);
    deviceManager.addChangeListener(this);
    mediaPlayer.addListener(this);
    masterLogger = juce::Logger::getCurrentLogger();

    currentLoop = new Loop;
    state = Stopped;
    gain = 1.0f;

    addAndMakeVisible(waveform);
    //[/Constructor]
}

AudioApp::~AudioApp()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    infoLabel = nullptr;
    appLabel = nullptr;
    loadButton = nullptr;
    playButton = nullptr;
    stopButton = nullptr;
    settingsButton = nullptr;
    loopButton = nullptr;
    gainSlider = nullptr;
    gainLabel = nullptr;
    shiftyLoopingButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    for (auto l : _crudeLoops) { l.next = nullptr; l.prev = nullptr; }
    currentLoop = nullptr;
    masterLogger = nullptr;
    if (mediaPlayer.hasStreamFinished()) mediaPlayer.removeListener(this);
    deviceManager.removeAudioCallback(&sourcePlayer);
    //[/Destructor]
}

//==============================================================================
void AudioApp::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff0f0f0f));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AudioApp::resized()
{
    infoLabel->setBounds (40, 544, 664, 120);
    appLabel->setBounds (40, 40, 168, 40);
    loadButton->setBounds (72, 384, 96, 56);
    playButton->setBounds (192, 384, 96, 56);
    stopButton->setBounds (320, 384, 96, 56);
    settingsButton->setBounds (440, 384, 96, 56);
    loopButton->setBounds (120, 224, 120, 40);
    gainSlider->setBounds (592, 240, 112, 160);
    gainLabel->setBounds (576, 416, 150, 24);
    shiftyLoopingButton->setBounds (384, 224, 112, 40);
    //[UserResized] Add your own custom resize handling here..
    waveform.setBounds(0, 0, 50, 20);

    //[/UserResized]
}

void AudioApp::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == loadButton)
    {
        //[UserButtonCode_loadButton] -- add your button handler code here..
        FileChooser chooser("Select a wav file to play", File::nonexistent, "*.wav");
        if (chooser.browseForFileToOpen()) {
            File file(chooser.getResult());
            mediaPlayer.setFile(file);

            AUDIO_FILENAME = file.getFullPathName().toUTF8();
          
            _crudeLoops = computeLoops(AUDIO_FILENAME);
            currentLoop = &_crudeLoops[rand() % _crudeLoops.size()];

            playButton->setEnabled(true);
            loopButton->setEnabled(true);
            shiftyLoopingButton->setEnabled(true);

        }
        //[/UserButtonCode_loadButton]
    }
    else if (buttonThatWasClicked == playButton)
    {
        //[UserButtonCode_playButton] -- add your button handler code here..

        if (Stopped == state || Paused == state) {
            changeState(Starting);
        } else if (Playing == state) {
            changeState(Pausing);
        } else if (Looping == state){
            changeState(Stopped);
        }
        //[/UserButtonCode_playButton]
    }
    else if (buttonThatWasClicked == stopButton)
    {
        //[UserButtonCode_stopButton] -- add your button handler code here..
        Paused == state ? changeState(Stopped) : changeState(Stopping);
        if (loopButton->getToggleState())
            loopButton->setToggleState(false, dontSendNotification);
        //[/UserButtonCode_stopButton]
    }
    else if (buttonThatWasClicked == settingsButton)
    {
        //[UserButtonCode_settingsButton] -- add your button handler code here..
        bool showMidiInputOptions = false;
        bool showMidiOutputSelector = false;
        bool showChnlsAsStereoPairs =true;
        bool hideAdvancedOptions = false;

        AudioDeviceSelectorComponent settings(deviceManager, 0,0,1,2,
                                              showMidiInputOptions,
                                              showMidiOutputSelector,
                                              showChnlsAsStereoPairs,
                                              hideAdvancedOptions);
        settings.setSize(500,400);
        DialogWindow::showModalDialog(String("Audio Settings"), &settings, TopLevelWindow::getTopLevelWindow(0), Colours::white, true);
        //[/UserButtonCode_settingsButton]
    }
    else if (buttonThatWasClicked == loopButton)
    {
        //[UserButtonCode_loopButton] -- add your button handler code here..
        (Looping != state) ? changeState(Looping) : changeState(Stopped);
        //[/UserButtonCode_loopButton]
    }
    else if (buttonThatWasClicked == shiftyLoopingButton)
    {
        //[UserButtonCode_shiftyLoopingButton] -- add your button handler code here..
       // mediaPlayer.stop();
        mediaPlayer.setLoopTimes(static_cast<double>(currentLoop->start), static_cast<double>(currentLoop->end));
        mediaPlayer.setPosition(static_cast<double>(currentLoop->start));
        changeState(ShiftyLooping);
        mediaPlayer.start();
        //[/UserButtonCode_shiftyLoopingButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void AudioApp::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == gainSlider)
    {
        //[UserSliderCode_gainSlider] -- add your slider handling code here..
        gain = static_cast<float>(gainSlider->getValue());
        sourcePlayer.setGain(gain);

        //[/UserSliderCode_gainSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void AudioApp::printCurrentState(juce::String s) {
    infoLabel->setText("Current State: " + s, sendNotification);
}

void AudioApp::changeListenerCallback(ChangeBroadcaster* src){

    if (&deviceManager == src) {
        AudioDeviceManager::AudioDeviceSetup setup;
        deviceManager.getAudioDeviceSetup(setup);
        setup.outputChannels.isZero() ? sourcePlayer.setSource(nullptr) :
                                        sourcePlayer.setSource(&mediaPlayer);
    }

}

void AudioApp::changeState(TransportState newState){

    if (state != newState) {
        state = newState;
        switch (state) {
            case Starting:
                printCurrentState(String("Starting..."));
                mediaPlayer.start();
                break;
            case Playing:
                printCurrentState(String("Playing..."));
                playButton->setButtonText("Pause");
                stopButton->setButtonText("Stop");
                stopButton->setEnabled(true);
                shiftyLoopingButton->setEnabled(true);
                break;
            case Pausing:
                printCurrentState(String("Pausing..."));
                mediaPlayer.stop();
                break;
            case Paused:
                printCurrentState(String("Paused"));
                playButton->setButtonText("Resume");
                stopButton->setButtonText("Return to Beggining");
                break;
            case Stopping:
                printCurrentState(String("Stopping..."));
                if (mediaPlayer.isLooping()) mediaPlayer.setLooping(false);
                mediaPlayer.stop();
                break;
            case Stopped:
                printCurrentState(String("Stopped"));
                playButton->setButtonText("Play");
                stopButton->setButtonText("Stop");
                stopButton->setEnabled(false);
                loopButton->setEnabled(true);
                mediaPlayer.setPosition(0.0);
                break;
            case Looping:
                printCurrentState(String("Looping..."));
                stopButton->setEnabled(true);
                playButton->setButtonText("Pause");
                stopButton->setButtonText("Stop");
                mediaPlayer.start();
                mediaPlayer.setLooping(true);
                break;
            case ShiftyLooping:
                printCurrentState(String("Shifty Looping..."));
                stopButton->setEnabled(true);
                playButton->setButtonText("Pause");
                stopButton->setButtonText("Stop");
                mediaPlayer.setLoopBetweenTimes(shiftyLoopingButton->getToggleState());
                currentLoop = currentLoop->next;
                break;
        }


    }

}

void AudioApp::playerStoppedOrStarted(drow::AudioFilePlayer* player){
    if (player == &mediaPlayer){
        gainSlider->setValue(static_cast<double>(sourcePlayer.getGain()));
        if (mediaPlayer.isPlaying()) {
            ShiftyLooping == state ? changeState(ShiftyLooping) : changeState(Playing);
        }
        else {
            if (Stopping == state || Playing == state) {
                changeState(Stopped);
            } else if (Pausing == state) changeState(Paused);
        }
    }
}

inline void AudioApp::shiftyLooping(){
        int r = rand() % 2;
        shifting = r == 0 ? true : false;
        r = rand() % 2;
        forward = r == 0 ? true : false;

        if (shifting) {
            masterLogger->writeToLog("Shifting...");
            if (forward) {
                masterLogger->writeToLog("forward");
                currentLoop = currentLoop->next;
                mediaPlayer.setLoopTimes(currentLoop->prev->end, currentLoop->end);
                mediaPlayer.setLoopBetweenTimes(true);
                mediaPlayer.start();
                mediaPlayer.setLoopTimes(currentLoop->start, currentLoop->end);
            } else {
                masterLogger->writeToLog("backwards");
                currentLoop = currentLoop->prev;
                mediaPlayer.setLoopTimes(currentLoop->next->start, currentLoop->end);
                mediaPlayer.setLoopBetweenTimes(true);
                mediaPlayer.start();
                mediaPlayer.setLoopTimes(currentLoop->start, currentLoop->end);
            }
        }

        playerStoppedOrStarted(&mediaPlayer);


}


void AudioApp::fileChanged(drow::AudioFilePlayer* player){}
void AudioApp::audioFilePlayerSettingChanged(drow::AudioFilePlayer* player, int settingCode){}
void AudioApp::timerCallback(){}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AudioApp" componentName=""
                 parentClasses="public Component, public ChangeListener, public ButtonListener"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ff0f0f0f"/>
  <LABEL name="Info Label" id="2fc17cbb62c7782f" memberName="infoLabel"
         virtualName="" explicitFocusOrder="0" pos="40 544 664 120" bkgCol="ff0d0d0d"
         textCol="ff87ee20" edTextCol="ff000000" edBkgCol="0" labelText="Data"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Apple LiSung" fontsize="17.899999999999998579" bold="0"
         italic="0" justification="33"/>
  <LABEL name="App Name Label" id="69243171a82e68ba" memberName="appLabel"
         virtualName="" explicitFocusOrder="0" pos="40 40 168 40" bkgCol="dfd3d3"
         textCol="fffdfdfb" edTextCol="ff000000" edBkgCol="0" labelText="SL480x"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Arial Black" fontsize="34.399999999999998579" bold="0"
         italic="0" justification="33"/>
  <TEXTBUTTON name="Load Button" id="a79aafbaa2e02f08" memberName="loadButton"
              virtualName="" explicitFocusOrder="0" pos="72 384 96 56" bgColOff="fff0f8ff"
              buttonText="Load File..." connectedEdges="3" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="Play Button" id="aa045a593e226508" memberName="playButton"
              virtualName="" explicitFocusOrder="0" pos="192 384 96 56" bgColOff="fff0f8ff"
              buttonText="Play" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Stop Button" id="e3f6aa07147c05" memberName="stopButton"
              virtualName="" explicitFocusOrder="0" pos="320 384 96 56" bgColOff="fff0f8ff"
              buttonText="Stop" connectedEdges="3" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="Settings Button" id="f5cdf9e0260e09a2" memberName="settingsButton"
              virtualName="" explicitFocusOrder="0" pos="440 384 96 56" bgColOff="fff0f8ff"
              buttonText="Settings..." connectedEdges="3" needsCallback="1"
              radioGroupId="0"/>
  <TOGGLEBUTTON name="Loop Button" id="a5b4e832ce1021b5" memberName="loopButton"
                virtualName="" explicitFocusOrder="0" pos="120 224 120 40" txtcol="ffe8d8d8"
                buttonText="Loop Sample" connectedEdges="15" needsCallback="1"
                radioGroupId="0" state="0"/>
  <SLIDER name="Gain Slider" id="9d008628b8772a4d" memberName="gainSlider"
          virtualName="" explicitFocusOrder="0" pos="592 240 112 160" thumbcol="ff7fffd4"
          trackcol="ff7fffd4" rotaryslideroutline="ff0000ff" min="0" max="1"
          int="0.10000000000000000555" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="2"/>
  <LABEL name="Gain Label" id="d9d519cd8dc043ff" memberName="gainLabel"
         virtualName="" explicitFocusOrder="0" pos="576 416 150 24" textCol="fff0ffff"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="ffff0000" labelText="GAIN"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Arial Black" fontsize="22.300000000000000711" bold="0"
         italic="0" justification="36"/>
  <TOGGLEBUTTON name="Shifty Button" id="717b6864ee8b9341" memberName="shiftyLoopingButton"
                virtualName="" explicitFocusOrder="0" pos="384 224 112 40" txtcol="fff3f3f3"
                buttonText="Shifty Looping" connectedEdges="0" needsCallback="1"
                radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
