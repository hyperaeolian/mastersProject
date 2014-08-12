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

    addAndMakeVisible (backgroundImg = new ImageComponent());
    backgroundImg->setName ("backgroundImg");


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

    //Audio device setup
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
    loadButton = nullptr;
    playButton = nullptr;
    stopButton = nullptr;
    settingsButton = nullptr;
    loopButton = nullptr;
    gainSlider = nullptr;
    gainLabel = nullptr;
    shiftyLoopingButton = nullptr;
    backgroundImg = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    for (auto l : crudeLoops) { l.next = nullptr; l.prev = nullptr; }
    currentLoop = nullptr;
    masterLogger = nullptr;
    similarity = nullptr;
    transMat = nullptr;
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
    loadButton->setBounds (72, 384, 96, 56);
    playButton->setBounds (192, 384, 96, 56);
    stopButton->setBounds (320, 384, 96, 56);
    settingsButton->setBounds (440, 384, 96, 56);
    loopButton->setBounds (120, 224, 120, 40);
    gainSlider->setBounds (592, 240, 112, 160);
    gainLabel->setBounds (576, 416, 150, 24);
    shiftyLoopingButton->setBounds (384, 224, 112, 40);
    backgroundImg->setBounds (16, 8, 680, 120);
    //[UserResized] Add your own custom resize handling here..
    addAndMakeVisible(waveform);
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

            crudeLoops = computeLoops(AUDIO_FILENAME);
            int n = random.nextInt(crudeLoops.size() - 1);
            currentLoop = &crudeLoops[n];

            similarity = new MATRIX(crudeLoops.size(), crudeLoops.size());
            computeDistances(crudeLoops, *similarity);
            transMat = new MATRIX(computeTransitionMatrix(*similarity));
            markov_chain = markov(*transMat, MarkovIterations, n);
            std::cout << "Markov Chain: " << markov_chain << std::endl;

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
        VAR start = currentLoop->start;
        VAR end = currentLoop->end;
        if (start > end) std::swap(start, end);
            mediaPlayer.setLoopTimes(static_cast<double>(start), static_cast<double>(end));
            mediaPlayer.setPosition(static_cast<double>(start));
            mediaPlayer.start();
        changeState(ShiftyLooping);
       // shiftyLoopingButton->triggerClick();
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
                //masterLogger->writeToLog("Current Loop: " + String(currentLoop->start) + " to " + String(currentLoop->end));
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

void AudioApp::shiftyLooping(){
    for (int i = 0; i < markov_chain.size(); ++i) {
        currentLoop = &crudeLoops[markov_chain[i]];
        std::thread t1(std::bind(&AudioApp::toggleLoop, this), std::ref(*currentLoop));
        t1.join();
        if (markov_chain[i+1] > markov_chain[i]) {
            shifting = forward = true;
            std::thread t1(std::bind(&AudioApp::toggleLoop, this), std::ref(*currentLoop));
            t1.join();
        } else if (markov_chain[i+1] < markov_chain[i]) {
            shifting = true;
            forward = false;
            std::thread t1(std::bind(&AudioApp::toggleLoop, this), std::ref(*currentLoop));
            t1.join();
        }
        
        if (ShiftyLooping != state) break;
    }

    
}

void AudioApp::toggleLoop(Loop &loop){
    if (shifting) {
        if (forward) {
            mediaPlayer.setLoopTimes(loop.prev->end, loop.end);
            mediaPlayer.setPosition(loop.prev->end);
        } else {
            mediaPlayer.setLoopTimes(loop.next->start, loop.end);
            mediaPlayer.setPosition(loop.next->start);
        }
    } else {
        mediaPlayer.setLoopTimes(loop.start, loop.end);
        mediaPlayer.setPosition(loop.start);
    }
    mediaPlayer.start();
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
                 parentClasses="public Component, public ChangeListener, public ButtonListener, public SliderListener, public Timer, public drow::AudioFilePlayer::Listener"
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
  <GENERICCOMPONENT name="backgroundImg" id="72aa7502c65b2396" memberName="backgroundImg"
                    virtualName="" explicitFocusOrder="0" pos="16 8 680 120" class="ImageComponent"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//==============================================================================
// Binary resources - be careful not to edit any of these sections!

// JUCER_RESOURCE: sl490x_png, 9107, "../../../Thesis/SL490x.png"
static const unsigned char resource_AudioApp_sl490x_png[] = { 137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,3,192,0,0,2,184,8,6,0,0,0,187,208,148,131,0,0,0,9,112,72,89,115,0,0,11,19,0,0,11,19,1,0,
154,156,24,0,0,0,25,116,69,88,116,83,111,102,116,119,97,114,101,0,65,100,111,98,101,32,73,109,97,103,101,82,101,97,100,121,113,201,101,60,0,0,35,32,73,68,65,84,120,218,236,221,91,172,28,119,125,192,241,
217,61,151,56,182,3,4,200,133,42,166,114,80,82,1,178,114,169,144,226,135,170,92,242,86,68,218,10,218,138,135,170,170,4,69,149,74,139,144,170,82,242,208,7,10,15,109,5,42,47,37,168,125,224,129,182,162,84,
4,65,159,128,62,180,85,144,16,66,209,169,138,146,72,137,144,77,46,142,19,226,216,177,29,159,203,118,126,227,249,175,255,103,206,204,238,236,57,123,124,59,159,143,52,218,227,61,123,153,157,93,75,231,187,
255,153,255,12,70,163,81,1,0,0,0,215,187,129,0,6,0,0,64,0,3,0,0,128,0,158,242,192,37,155,23,0,0,128,237,24,237,66,172,206,45,128,5,47,0,0,0,87,115,16,239,40,128,123,68,239,96,202,191,1,0,0,216,227,109,
219,243,186,29,199,240,182,2,184,35,124,7,45,145,59,107,32,3,0,0,176,55,163,183,237,247,163,73,183,159,53,132,103,14,224,150,248,29,180,196,239,160,229,119,194,23,0,0,128,105,33,60,234,8,224,214,24,158,
37,130,123,7,240,132,240,29,180,252,123,208,18,191,109,49,44,132,1,0,0,132,239,168,113,221,70,75,252,142,186,66,184,111,4,247,10,224,182,248,189,255,254,251,255,160,188,250,161,242,231,95,47,151,55,122,
255,0,0,0,232,85,191,163,209,255,172,175,175,255,199,137,19,39,254,249,185,231,158,123,101,66,236,110,52,46,187,98,184,87,4,79,13,224,70,252,14,238,187,239,190,123,135,195,225,191,151,63,255,178,183,13,
0,0,128,29,56,125,225,194,133,191,93,89,89,121,164,142,220,34,139,221,245,44,126,243,101,212,114,219,94,17,60,75,0,167,81,223,127,44,170,53,25,20,39,139,197,226,212,104,161,56,87,12,189,109,0,0,0,76,181,
80,246,234,27,203,162,188,109,176,90,44,215,237,186,182,182,246,205,199,31,127,252,211,141,248,77,193,187,158,93,174,23,91,71,133,123,71,240,196,0,206,71,127,203,248,253,173,242,159,223,140,159,79,149,
171,124,108,180,92,69,48,0,0,0,108,199,45,197,90,241,75,131,11,41,130,191,85,70,240,95,52,98,55,45,107,45,215,109,204,26,193,157,1,156,199,239,145,35,71,110,94,94,94,126,166,252,241,13,47,23,139,85,252,
2,0,0,192,78,189,185,108,219,67,117,4,159,57,115,230,175,158,120,226,137,111,101,209,155,47,171,89,8,175,21,155,71,134,55,29,19,220,21,192,195,105,241,27,255,44,227,247,139,17,191,177,171,179,248,5,0,
0,96,94,98,144,245,133,98,169,250,121,255,254,3,127,92,94,28,44,151,3,245,178,191,92,246,149,203,13,245,18,65,186,88,47,11,117,211,198,178,233,172,67,45,19,57,119,7,112,30,191,239,126,247,187,111,46,47,
127,51,254,241,236,104,201,187,3,0,0,192,92,61,95,182,230,133,178,95,135,195,193,173,239,120,199,59,126,59,139,224,253,141,8,142,0,94,154,16,192,19,143,211,221,18,192,205,82,94,94,94,126,111,121,241,134,
88,153,51,213,227,3,0,0,192,124,189,90,247,230,190,125,251,126,181,188,184,169,17,193,55,246,136,224,162,152,50,10,188,56,225,249,7,245,125,238,201,87,6,0,0,0,230,45,206,48,244,214,193,90,49,28,14,111,
175,3,248,245,58,116,83,228,166,160,109,158,26,169,121,222,224,65,145,77,136,213,55,128,199,17,28,63,152,241,25,0,0,128,221,146,78,175,187,188,188,252,43,117,0,167,81,222,182,248,93,111,92,230,187,63,
247,11,224,230,228,87,69,161,122,1,0,0,216,125,141,65,215,216,253,185,109,228,119,189,99,89,200,66,120,28,193,209,184,249,140,208,93,35,192,227,3,136,203,219,118,77,160,5,0,0,0,187,97,127,177,249,216,
222,252,220,192,205,211,34,45,212,215,15,179,72,110,213,123,23,104,0,0,0,184,204,1,156,199,111,138,222,152,4,43,78,28,188,84,255,59,159,9,58,111,217,81,223,0,22,189,0,0,0,92,41,251,58,226,247,66,113,105,
22,232,248,121,161,104,63,29,210,228,99,128,91,246,115,238,117,30,37,0,0,0,152,179,27,138,75,187,51,175,21,151,70,124,151,235,142,93,108,196,239,160,171,99,243,227,128,219,70,128,7,226,23,0,0,128,43,104,
169,142,223,165,108,89,174,131,183,25,191,249,50,113,55,232,225,148,39,173,38,193,178,237,1,0,0,184,204,1,188,152,93,46,182,196,111,219,174,207,69,49,97,32,119,82,0,79,189,51,0,0,0,236,130,133,198,178,
216,114,221,76,241,59,45,128,197,47,0,0,0,87,194,176,37,118,155,187,60,15,138,25,15,225,29,218,174,0,0,0,92,101,6,89,228,230,209,187,208,184,190,25,191,205,99,128,167,6,240,196,59,0,0,0,192,46,155,52,
194,219,53,234,59,151,17,96,17,12,0,0,192,229,212,21,187,93,49,220,171,97,251,76,130,5,0,0,0,87,42,130,139,150,216,221,214,105,123,29,3,12,0,0,192,213,28,193,125,194,119,208,113,217,43,128,157,2,9,0,0,
128,43,29,191,147,254,93,204,248,123,35,192,0,0,0,236,13,2,24,0,0,128,171,221,92,246,78,22,192,0,0,0,236,9,2,24,0,0,0,1,12,0,0,0,87,208,182,78,119,36,128,1,0,0,216,211,4,48,0,0,0,2,24,0,0,0,4,48,0,0,0,
8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,64,0,3,0,0,128,0,6,0,0,
0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,16,192,0,
0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,
0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,
0,0,2,24,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,
0,64,0,3,0,0,32,128,109,2,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,
0,3,0,0,128,0,6,0,0,0,1,12,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,
0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,
0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,
32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,1,12,0,0,0,2,24,0,0,0,4,
48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,32,128,1,0,0,64,0,3,0,0,128,
0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,8,96,0,0,0,16,
192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,2,
24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,
12,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,
1,0,0,64,0,3,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,
0,0,0,8,96,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,
0,0,0,1,12,0,0,0,2,24,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,
0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,
0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,
0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,2,216,38,0,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,
0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,4,48,0,0,0,8,96,0,0,0,
16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,1,12,0,0,0,
2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,
64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,
32,128,1,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,
0,4,48,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,
128,0,6,0,0,0,1,12,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,
192,0,0,0,32,128,1,0,0,64,0,3,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,
0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,
0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,
0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,
0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,8,96,0,0,0,16,192,
0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,2,24,0,
0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,32,128,1,0,0,64,0,3,0,0,128,0,6,0,0,0,1,12,0,0,0,2,24,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,176,107,22,119,243,193,143,220,253,74,241,224,
3,207,21,135,239,56,83,220,246,150,243,197,11,47,237,43,78,148,203,15,31,191,165,248,222,15,111,223,116,187,135,255,104,165,250,253,39,63,255,158,137,143,215,118,187,191,255,203,31,85,143,255,185,175,
28,41,86,158,124,211,150,235,187,196,227,124,233,107,239,44,190,240,169,159,244,126,77,241,248,95,255,238,225,226,225,79,172,20,7,110,92,219,242,156,109,235,251,218,185,197,226,51,95,188,175,122,190,54,
93,235,63,205,131,15,60,95,60,112,207,139,197,173,229,125,211,246,125,230,248,193,114,219,190,173,215,227,220,89,190,47,31,56,250,92,117,25,75,172,231,211,199,14,22,43,79,221,92,124,239,177,219,171,127,
183,221,231,163,191,241,76,113,96,255,90,245,115,242,116,249,188,177,196,123,219,124,238,105,239,219,15,31,127,107,241,197,242,125,232,146,110,23,219,48,158,99,210,231,35,182,199,157,245,231,45,222,159,
174,207,28,0,0,32,128,231,230,99,31,121,170,10,180,220,193,50,154,110,123,203,43,85,168,172,60,245,166,45,65,56,41,86,39,221,174,235,126,233,250,8,185,136,161,190,166,221,62,34,44,34,239,129,123,78,22,
15,189,239,88,103,108,70,40,134,199,202,192,235,138,223,89,94,119,30,122,31,251,240,83,155,94,95,122,156,88,98,189,98,157,190,250,111,119,117,62,111,219,251,147,30,59,150,135,222,127,172,248,250,119,14,
111,137,198,8,223,248,125,250,2,33,61,119,138,232,120,204,71,127,112,71,245,37,65,223,247,45,214,183,250,76,116,108,199,105,219,167,185,61,218,182,73,220,230,133,151,247,205,244,5,195,213,36,125,30,219,
190,148,216,174,244,165,9,0,0,8,224,29,136,8,74,113,245,213,111,220,181,41,162,34,146,170,24,185,140,127,120,199,232,226,164,240,249,221,79,255,218,150,160,138,17,203,182,223,37,143,254,224,208,56,220,
218,226,45,174,75,163,170,17,146,243,146,175,91,26,141,78,163,162,17,73,15,30,125,190,138,215,184,221,103,203,219,197,168,105,51,154,226,254,241,251,184,62,94,71,62,218,155,70,120,171,168,44,35,57,130,
55,130,182,77,62,154,27,207,29,207,251,208,251,143,87,75,140,34,207,18,155,17,176,147,70,255,39,125,214,98,61,211,23,19,223,127,236,109,91,190,92,73,159,185,24,221,190,22,165,247,107,214,61,4,186,196,
246,248,179,223,255,105,21,192,93,159,111,0,0,184,30,237,202,49,192,177,91,109,136,56,107,142,32,70,164,116,5,213,181,228,233,106,87,227,139,175,45,70,129,219,130,46,133,242,188,70,237,34,50,63,85,134,
75,136,231,142,32,202,119,9,190,24,180,119,20,159,251,135,35,213,207,17,56,31,175,227,48,73,113,27,191,143,219,197,237,243,245,139,199,139,199,77,175,45,110,159,239,234,220,165,10,253,242,253,78,129,22,
187,190,247,21,247,137,117,141,112,158,245,203,128,20,191,241,58,34,246,99,189,155,95,174,164,207,220,60,71,79,47,167,52,226,62,47,7,170,61,49,206,23,0,0,32,128,231,32,5,211,245,190,123,229,183,235,184,
77,163,192,73,140,74,166,221,75,231,25,251,31,253,224,51,227,227,90,99,100,125,82,156,167,81,231,24,165,78,239,71,30,153,241,251,73,199,211,198,227,167,247,47,70,118,251,138,227,143,195,225,30,209,156,
60,250,159,135,198,207,51,75,152,165,47,25,34,122,219,118,185,6,0,0,200,237,202,144,88,132,85,68,215,209,123,94,172,38,56,186,94,69,32,198,238,195,213,110,191,217,177,192,31,170,131,113,222,81,118,180,
140,217,240,253,58,50,39,135,232,237,227,96,142,137,161,226,61,121,160,190,127,68,123,159,9,161,226,121,98,4,56,238,23,143,211,103,4,53,63,246,182,247,231,229,216,193,234,115,18,207,19,207,55,105,66,172,
36,110,27,207,49,239,93,204,147,127,250,220,99,213,107,254,195,135,143,214,95,28,28,27,79,230,22,239,115,219,164,90,105,247,244,116,12,116,218,118,241,218,242,207,66,154,188,236,206,67,103,170,231,72,
147,135,53,39,30,139,209,254,244,158,165,127,167,99,129,155,187,67,247,121,204,184,62,94,87,254,28,255,250,119,255,53,254,44,199,46,232,105,119,235,120,109,109,95,178,164,93,206,227,241,99,196,61,214,
47,214,43,77,100,150,214,35,29,230,240,76,53,250,126,168,243,203,150,216,158,241,255,229,200,93,175,84,63,167,245,190,30,246,18,1,0,224,234,179,43,35,192,113,28,102,138,148,79,213,199,26,94,175,30,205,
70,129,211,177,166,151,34,105,126,241,31,143,155,226,167,239,227,62,86,223,46,141,0,31,185,235,23,213,101,223,227,72,243,231,137,176,234,35,61,215,164,209,229,182,8,74,129,152,142,171,158,254,101,192,
139,227,215,178,27,187,54,167,109,29,95,58,196,44,225,177,94,241,60,233,189,142,8,76,187,163,111,185,239,254,181,42,36,227,139,145,252,179,31,63,199,99,197,125,227,49,206,156,93,28,127,89,20,177,252,229,
207,254,104,211,238,230,17,144,249,118,76,183,207,175,155,245,49,227,119,113,155,252,223,177,156,168,71,251,35,62,211,235,110,155,8,238,129,108,187,231,95,120,164,221,243,99,61,110,173,67,54,77,200,150,
182,95,219,103,58,102,248,142,104,142,231,79,187,194,199,122,167,227,220,1,0,96,158,118,101,4,56,70,143,14,31,58,93,143,6,157,28,143,130,245,57,61,207,44,179,53,95,13,210,68,82,241,71,123,140,18,222,90,
7,207,188,71,127,243,89,128,251,238,90,126,226,165,27,171,203,180,78,233,242,233,227,55,245,186,127,154,229,57,158,251,182,55,151,81,223,227,62,31,168,143,253,157,117,178,166,180,187,120,68,99,108,203,
207,60,121,223,196,219,207,250,90,182,43,130,174,218,197,250,59,135,199,177,151,70,65,171,89,192,203,245,109,142,86,166,120,140,235,35,40,211,251,21,19,79,69,140,54,103,232,142,232,139,221,185,171,17,
228,79,172,20,127,242,215,239,25,31,83,93,124,247,210,40,109,220,167,185,93,103,125,204,24,181,205,39,82,139,127,55,191,52,73,123,14,196,237,242,47,65,210,117,213,151,92,141,189,16,210,113,229,49,10,156,
238,19,183,255,120,189,157,226,50,70,131,243,245,139,96,78,247,73,175,43,133,244,197,153,200,143,27,9,6,0,224,234,15,224,234,143,245,111,220,85,253,241,159,38,81,74,33,60,237,244,60,177,139,230,188,229,
187,142,54,215,113,30,231,134,189,184,27,244,177,241,40,87,4,192,44,35,160,125,164,145,196,237,124,65,144,238,187,157,145,248,244,124,49,170,57,73,4,75,188,215,105,183,228,248,82,96,86,113,159,152,197,
58,62,47,211,226,103,218,196,92,105,178,175,77,95,86,156,93,172,118,29,158,69,245,121,109,236,10,28,159,153,91,223,114,174,158,241,250,216,150,245,76,241,219,220,237,57,214,57,62,247,17,124,249,168,117,
186,46,70,74,99,251,197,54,232,19,126,187,241,152,241,24,17,193,23,103,114,127,110,83,0,167,93,240,99,155,180,253,255,141,47,9,242,219,167,184,77,231,113,142,47,71,210,54,137,221,158,99,59,229,19,167,
165,251,196,117,95,184,251,39,213,237,5,48,0,0,243,52,220,205,7,143,63,108,99,132,41,205,206,155,66,41,78,207,179,157,24,187,90,103,241,109,6,223,35,19,38,168,218,246,115,212,187,173,110,103,27,164,88,
217,206,125,211,125,218,130,39,70,38,99,137,47,45,98,68,49,197,88,154,133,122,59,207,149,142,231,125,168,14,164,105,175,105,82,244,167,115,19,167,159,183,51,155,242,247,58,142,183,78,35,160,213,232,120,
203,103,185,249,5,64,218,117,56,2,177,109,219,196,117,41,30,211,174,234,211,236,198,99,86,175,173,62,132,33,237,206,191,245,249,110,105,125,174,174,47,147,198,235,208,152,40,46,125,121,212,20,95,30,165,
89,204,175,181,61,66,0,0,184,186,93,150,162,172,142,51,44,163,48,254,176,142,221,49,211,196,55,109,147,236,76,58,47,105,190,235,230,44,166,157,7,120,94,175,113,55,67,253,133,151,247,109,10,174,62,187,
65,223,121,199,233,234,50,29,223,25,147,77,197,54,140,209,203,62,242,0,105,123,77,177,77,99,100,56,226,60,214,111,229,201,155,119,124,220,115,68,84,156,70,171,58,134,245,131,207,84,159,145,180,27,118,
46,94,83,172,95,215,107,201,39,210,138,199,138,145,208,36,77,6,213,22,213,205,115,17,119,189,151,249,246,191,181,229,253,104,222,47,29,67,61,105,151,237,244,187,222,199,91,239,194,99,142,255,191,214,199,
17,199,94,13,49,10,155,118,127,78,35,196,77,147,66,53,206,9,29,163,229,249,108,228,201,180,61,62,210,196,88,0,0,112,205,4,112,254,135,117,58,94,54,102,125,101,134,109,119,236,82,4,196,182,123,225,165,
233,187,110,167,17,183,8,144,180,253,227,186,216,149,245,171,61,70,169,243,89,163,219,190,64,152,117,119,226,190,98,221,34,88,99,148,48,78,53,21,113,217,220,229,121,214,215,210,140,211,121,76,80,214,22,
230,215,139,248,178,234,206,234,248,221,23,171,0,206,119,127,158,245,11,158,230,151,40,249,54,155,246,62,92,239,167,82,3,0,224,58,14,224,20,46,225,224,126,187,54,110,39,218,34,74,99,244,124,218,177,203,
17,143,205,89,163,99,215,213,24,137,139,235,227,247,211,30,35,77,104,245,216,101,62,149,85,124,70,98,221,98,29,63,212,113,14,226,216,5,57,189,150,237,76,150,212,231,84,75,205,88,203,229,163,227,39,122,
68,90,26,125,143,81,249,174,232,75,35,246,249,151,29,151,251,49,147,52,25,86,218,141,60,237,254,220,181,75,248,164,40,110,174,67,30,181,143,212,35,252,0,0,112,57,12,47,247,19,166,63,164,103,253,131,156,
75,199,149,86,51,252,126,228,169,9,193,113,113,247,225,139,193,114,251,56,56,46,158,99,245,98,40,165,184,233,18,143,159,38,180,250,246,54,38,180,218,169,52,235,114,58,86,180,41,94,211,248,181,212,231,
219,221,13,233,116,75,93,95,14,60,157,205,108,60,73,26,133,143,201,168,218,162,186,250,82,226,232,243,227,47,42,250,196,248,78,31,115,82,120,230,187,58,199,46,233,233,188,190,93,135,18,228,51,68,111,253,
63,127,114,211,250,198,99,167,47,194,142,238,210,251,6,0,0,151,45,128,227,56,221,8,151,252,88,191,52,82,151,130,230,209,255,60,100,235,207,40,162,33,205,162,27,219,49,77,60,213,220,198,113,156,117,252,
28,183,111,238,30,28,35,110,17,50,241,251,184,93,26,69,205,227,57,189,127,249,237,47,183,124,98,177,174,80,143,117,75,33,21,51,125,167,211,231,52,99,240,214,29,156,135,58,157,234,40,23,219,38,93,215,119,
182,235,152,236,41,223,238,205,243,3,167,247,44,110,211,28,153,79,209,217,140,241,237,62,102,26,177,158,20,173,33,77,134,149,62,11,223,239,24,253,77,210,246,207,63,143,233,60,224,213,36,89,217,132,87,
105,187,197,23,45,109,95,114,152,0,11,0,128,221,48,247,125,15,227,15,215,248,35,120,210,31,214,205,83,159,112,41,32,186,254,232,143,153,180,47,30,67,125,71,53,233,84,140,224,198,54,142,211,197,180,29,
139,26,163,163,109,179,81,167,115,193,166,88,137,209,211,88,210,239,242,227,53,31,169,78,101,245,214,43,182,61,226,181,198,72,107,215,140,225,177,142,49,227,116,108,139,252,156,211,243,20,159,211,116,
142,231,23,234,137,183,210,54,138,207,113,223,237,19,235,250,165,175,189,115,124,222,222,56,53,80,126,78,220,244,5,199,151,90,118,205,142,209,219,120,175,226,181,165,73,163,226,61,140,251,111,231,49,227,
54,105,146,171,52,169,92,188,206,230,49,221,249,100,88,233,51,53,233,245,197,227,198,227,165,243,71,167,251,165,247,41,31,113,142,199,138,237,23,219,54,34,56,222,195,124,221,99,27,207,235,52,101,0,0,176,
107,1,92,157,6,167,252,67,58,118,117,78,199,15,230,127,76,199,168,82,215,172,174,125,71,26,155,183,75,97,210,118,187,219,118,48,250,183,157,145,207,237,222,103,150,17,175,136,130,234,92,173,71,159,175,
78,111,115,105,54,224,131,245,110,206,183,76,252,130,161,10,146,242,61,138,168,138,115,189,30,174,79,21,116,166,12,235,216,53,61,118,85,141,209,186,73,187,200,206,99,219,244,121,127,226,156,209,147,102,
254,142,117,140,80,138,221,180,35,16,99,123,196,136,111,30,128,39,170,224,187,105,91,49,31,123,42,196,242,208,251,142,141,183,115,108,219,184,174,107,27,119,109,155,88,151,8,215,244,190,165,89,149,227,
113,226,61,235,138,189,184,62,102,219,78,95,6,228,255,127,182,251,152,17,164,31,47,195,51,125,81,149,102,25,111,91,231,248,63,28,219,110,210,123,30,159,221,88,143,24,25,79,255,247,211,110,234,49,218,219,
246,89,138,47,56,98,61,171,93,172,239,122,165,186,79,10,233,184,126,229,41,95,146,1,0,48,95,131,209,104,116,241,135,82,125,221,176,94,22,202,101,233,222,123,239,125,120,97,97,225,207,95,40,150,138,231,
71,75,182,24,123,66,156,223,184,10,197,175,28,217,179,123,43,68,212,126,249,179,63,26,199,109,219,23,87,249,169,201,38,157,194,12,0,0,250,184,103,112,182,186,252,241,143,127,252,205,242,226,245,114,137,
243,158,190,86,46,49,179,234,171,229,114,42,91,94,173,175,127,173,190,93,220,254,66,185,172,150,203,122,185,108,212,75,49,170,195,119,104,19,3,109,98,132,56,226,55,70,113,157,139,23,0,128,235,129,243,
143,0,155,84,179,140,127,248,169,241,238,212,143,204,120,158,101,0,0,16,192,192,53,17,191,49,153,86,104,155,188,10,0,0,4,48,92,103,118,58,129,218,181,252,186,99,121,166,62,229,214,118,39,166,3,0,0,1,12,
215,136,79,126,254,61,94,123,15,49,65,152,201,175,0,0,184,86,152,4,11,0,0,0,1,12,0,0,0,2,24,0,0,0,4,48,0,0,0,236,216,168,94,4,48,0,0,0,8,96,0,0,0,16,192,0,0,0,92,67,230,178,27,244,112,202,131,143,108,
103,0,0,0,174,7,70,128,1,0,0,184,226,110,44,54,170,203,213,213,213,115,245,85,205,1,217,105,3,180,83,7,112,135,211,238,60,26,141,78,197,229,190,122,101,0,0,0,96,222,150,235,126,45,3,248,108,163,75,243,
153,160,155,63,55,227,119,226,222,204,83,71,128,207,159,63,255,223,113,121,176,12,224,5,123,68,3,0,0,176,11,222,48,88,79,13,122,170,35,116,71,45,65,220,25,187,125,3,120,211,3,61,241,196,19,43,163,209,
232,120,196,239,155,139,117,239,10,0,0,0,115,181,92,245,230,90,245,243,201,147,39,143,245,136,223,73,35,189,163,89,2,120,203,157,87,87,87,255,38,126,184,109,176,58,222,47,27,0,0,0,230,225,208,224,245,
234,242,236,217,179,47,159,62,125,250,23,117,196,110,180,44,121,0,111,20,51,142,4,79,11,224,234,1,86,86,86,254,101,52,26,253,52,70,129,15,13,46,140,247,205,6,0,0,128,157,197,239,133,234,144,219,141,141,
141,181,227,199,143,255,111,35,110,155,241,219,21,195,147,142,11,238,29,192,227,59,253,236,103,63,251,157,242,135,51,49,2,124,247,224,252,120,120,26,0,0,0,102,213,108,203,103,159,125,246,255,78,159,62,
253,74,22,184,235,141,224,93,207,174,27,181,92,78,29,169,93,156,22,190,233,1,95,122,233,165,87,214,215,215,223,123,248,240,225,175,47,12,135,119,71,165,223,86,172,22,175,22,11,197,153,209,130,163,131,
1,0,0,152,234,224,96,163,56,80,22,228,193,250,240,218,24,249,61,118,236,216,227,39,79,158,124,54,139,220,88,214,26,203,122,99,153,52,18,220,106,48,26,93,252,253,160,148,93,63,172,151,197,122,89,46,151,
27,170,72,47,138,3,239,122,215,187,254,116,223,190,125,191,87,222,101,191,183,15,0,0,128,237,56,117,234,212,241,159,255,252,231,79,158,59,119,238,76,29,181,171,245,18,7,5,159,171,151,51,245,242,106,185,
156,206,46,211,245,113,155,243,229,114,33,139,229,60,142,227,244,190,163,77,1,220,136,224,97,182,44,213,75,10,224,136,222,3,17,238,111,127,251,219,31,220,191,127,255,253,75,75,75,119,150,143,179,80,94,
183,80,94,14,179,203,65,99,41,234,39,31,120,171,1,0,0,246,158,56,207,111,25,188,175,190,252,242,203,207,213,225,155,118,99,78,1,124,161,35,128,83,252,166,159,95,171,151,115,245,237,219,2,184,232,19,192,
131,58,126,23,234,101,185,94,246,101,17,124,48,91,110,170,47,15,212,203,141,117,48,223,80,199,243,66,182,108,9,98,0,0,0,246,148,230,108,206,109,1,28,163,186,103,235,200,77,209,155,143,252,190,86,255,254,
124,125,251,213,226,210,174,210,227,0,30,101,209,187,216,99,165,138,98,243,1,199,169,168,47,100,203,235,245,99,45,20,151,38,214,74,79,184,214,248,221,48,139,223,230,37,0,0,0,215,119,248,54,91,115,148,
181,230,106,214,153,105,215,230,115,89,228,166,223,173,53,98,183,215,100,88,139,51,212,248,70,246,36,171,29,225,59,200,238,159,162,121,41,187,77,26,1,206,103,159,238,138,95,81,12,0,0,112,237,199,238,164,
8,110,235,205,230,8,112,30,192,205,8,206,39,197,106,11,236,238,0,142,161,225,198,100,88,109,67,210,41,128,23,138,238,145,221,180,242,105,215,233,124,23,232,252,216,96,1,12,0,0,176,247,2,56,31,1,206,3,
184,57,2,220,92,82,0,175,22,155,71,129,119,52,2,156,86,106,208,136,224,60,128,135,197,214,145,223,230,109,34,126,151,58,66,185,45,128,69,47,0,0,192,245,29,195,125,70,128,99,57,95,92,218,5,186,185,27,116,
154,45,186,237,116,72,227,231,200,143,255,237,19,192,105,133,6,245,3,15,234,21,74,17,59,108,68,107,218,119,59,85,251,210,148,0,118,12,48,0,0,192,222,11,225,188,55,83,71,230,167,65,74,187,65,191,158,197,
111,51,128,155,231,3,222,104,137,237,98,98,0,55,118,131,78,163,192,121,4,15,235,39,203,3,182,57,109,117,58,111,211,114,113,233,92,194,121,0,23,133,217,160,1,0,0,246,106,4,55,231,157,202,143,229,189,208,
18,193,249,146,239,2,157,79,130,149,47,253,2,184,99,197,138,70,4,23,197,214,145,223,230,180,213,41,126,151,26,241,219,247,24,96,0,0,0,246,70,0,55,207,60,148,31,11,220,182,228,199,255,166,8,46,242,248,
109,238,254,220,25,192,45,163,192,109,1,220,22,191,105,37,22,235,149,234,154,40,203,232,47,0,0,128,8,110,219,13,122,173,216,124,60,240,106,99,73,129,220,28,253,221,40,38,140,254,118,6,112,203,138,21,197,
165,93,161,155,218,14,90,206,79,123,212,156,253,185,237,60,192,0,0,0,236,157,248,205,123,178,57,25,86,51,130,243,224,109,27,249,221,50,251,115,219,232,239,196,0,110,25,5,222,168,227,117,163,165,216,211,
239,23,234,149,200,163,183,109,215,103,163,191,0,0,0,66,184,237,88,224,230,49,193,205,243,253,54,39,190,234,21,191,97,48,225,119,23,111,176,249,188,192,249,8,110,126,185,208,184,28,22,221,199,253,26,253,
5,0,0,16,191,249,207,109,35,193,27,29,193,219,118,222,223,93,9,224,60,130,139,150,24,94,200,254,221,252,93,215,200,175,16,6,0,0,216,155,241,91,20,237,199,3,231,199,5,231,97,220,22,190,189,226,183,87,0,
79,136,224,174,101,216,17,201,69,209,61,250,43,130,1,0,0,246,102,252,22,45,65,155,66,183,104,9,223,162,104,153,240,106,212,35,110,123,5,112,207,16,46,26,225,219,181,203,179,248,5,0,0,16,193,109,17,220,
21,195,109,191,155,41,126,103,14,224,142,8,110,139,221,162,152,126,188,175,240,5,0,0,16,194,205,235,70,83,194,120,230,240,221,118,0,247,8,225,162,152,109,162,43,33,12,0,0,176,183,195,183,235,247,163,174,
251,140,182,17,179,219,14,224,41,33,60,41,112,5,47,0,0,0,93,65,60,49,82,71,59,136,216,29,7,240,140,49,12,0,0,0,179,21,242,156,194,117,174,1,44,136,1,0,0,184,90,130,247,178,6,48,0,0,0,92,45,4,48,0,0,0,
123,194,255,11,48,0,170,117,220,122,207,109,67,1,0,0,0,0,73,69,78,68,174,66,96,130,0,0};

const char* AudioApp::sl490x_png = (const char*) resource_AudioApp_sl490x_png;
const int AudioApp::sl490x_pngSize = 9107;


//[EndFile] You can add extra defines here...
//[/EndFile]
