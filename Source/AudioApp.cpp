/*
  ==============================================================================

Main App Functions

  ==============================================================================
*/

//[Headers]
#include "MainComponent.h"
#include "LoopGen.h"
//#include "LoopGenerator.h"
//#include "LoopGenerator.cpp"
//[/Headers]

#include "AudioApp.h"


//[MiscUserDefs]
//[/MiscUserDefs]

//==============================================================================
AudioApp::AudioApp () : APP_WIDTH(900), APP_HEIGHT(400)
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

    addAndMakeVisible (mainGroup = new GroupComponent ("Buttons",
                                                       TRANS("Main")));
    mainGroup->setColour (GroupComponent::outlineColourId, Colour (0x66121111));
    mainGroup->setColour (GroupComponent::textColourId, Colour (0xfff5f4f4));

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
    gainSlider->setRange (0, 1, 0.2);
    gainSlider->setSliderStyle (Slider::LinearVertical);
    gainSlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    gainSlider->setColour (Slider::thumbColourId, Colours::aquamarine);
    gainSlider->setColour (Slider::trackColourId, Colours::white);
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

    addAndMakeVisible (delaySlider = new Slider ("Delay Slider"));
    delaySlider->setRange (0, 1, 0.2);
    delaySlider->setSliderStyle (Slider::LinearVertical);
    delaySlider->setTextBoxStyle (Slider::TextBoxBelow, false, 80, 20);
    delaySlider->setColour (Slider::thumbColourId, Colours::aquamarine);
    delaySlider->setColour (Slider::trackColourId, Colours::white);
    delaySlider->setColour (Slider::rotarySliderOutlineColourId, Colours::blue);
    delaySlider->addListener (this);
    delaySlider->setSkewFactor (2);

    addAndMakeVisible (delayLabel = new Label ("Delay Label",
                                               TRANS("DELAY")));
    delayLabel->setFont (Font ("Arial Black", 22.30f, Font::plain));
    delayLabel->setJustificationType (Justification::centred);
    delayLabel->setEditable (false, false, false);
    delayLabel->setColour (Label::textColourId, Colours::azure);
    delayLabel->setColour (TextEditor::textColourId, Colours::black);
    delayLabel->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    delayLabel->setColour (TextEditor::highlightColourId, Colours::red);

    addAndMakeVisible (shiftButton = new ToggleButton ("Shifty Button"));
    shiftButton->setButtonText (TRANS("Shifty"));
    shiftButton->addListener (this);
    shiftButton->setColour (ToggleButton::textColourId, Colours::grey);


    //[UserPreSize]
    setSize(APP_WIDTH, APP_HEIGHT);
    //[/UserPreSize]

    //setSize (600, 400);


    //[Constructor]
    playButton->setEnabled(false);
    stopButton->setEnabled(false);
    shiftButton->setEnabled(false);
    formatManager.registerBasicFormats();
    sourcePlayer.setSource(&transportSource);
    deviceManager.addAudioCallback(&sourcePlayer);
    deviceManager.initialise(0, 2, nullptr, true);
    deviceManager.addChangeListener(this);
    transportSource.addChangeListener(this);
    state = Stopped;
    gain = 0.0f;
    forward = false;
    
    
    //[/Constructor]
}

AudioApp::~AudioApp()
{
    //[Destructor_pre].
    //[/Destructor_pre]

    infoLabel = nullptr;
    appLabel = nullptr;
    mainGroup = nullptr;
    loadButton = nullptr;
    playButton = nullptr;
    stopButton = nullptr;
    settingsButton = nullptr;
    loopButton = nullptr;
    gainSlider = nullptr;
    gainLabel = nullptr;
    delaySlider = nullptr;
    delayLabel = nullptr;
    shiftButton = nullptr;


    //[Destructor].
    //[/Destructor]
}

//==============================================================================
void AudioApp::paint (Graphics& g)
{
    //[UserPrePaint]
    //[/UserPrePaint]

    g.fillAll (Colour (0xff0f0f0f));

    g.setGradientFill (ColourGradient (Colour (0xff262e90),
                                       50.0f, 50.0f,
                                       Colour (0xff4f0080),
                                       416.0f, 288.0f,
                                       false));
    g.fillRoundedRectangle (20.0f, 12.0f, 708.0f, 668.0f, 10.000f);

    //[UserPaint]
    //[/UserPaint]
}

void AudioApp::resized()
{
    infoLabel->setBounds (40, 544, 664, 120);
    appLabel->setBounds (40, 40, 168, 40);
    mainGroup->setBounds (48, 104, 528, 392);
    loadButton->setBounds (72, 384, 96, 56);
    playButton->setBounds (192, 384, 96, 56);
    stopButton->setBounds (320, 384, 96, 56);
    settingsButton->setBounds (440, 384, 96, 56);
    loopButton->setBounds (600, 96, 120, 40);
    gainSlider->setBounds (592, 328, 112, 160);
    gainLabel->setBounds (568, 496, 150, 24);
    delaySlider->setBounds (592, 128, 112, 160);
    delayLabel->setBounds (576, 296, 150, 24);
    shiftButton->setBounds (472, 136, 72, 88);
    //[UserResized]
    //[/UserResized]
}

void AudioApp::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == loadButton)
    {
        //[UserButtonCode_loadButton]
        FileChooser chooser("Select a wav file to play", File::nonexistent, "*.wav");
        if (chooser.browseForFileToOpen()) {
            File file(chooser.getResult());
            AUDIO_FILENAME = file.getFullPathName().toUTF8();
            readerSource = new AudioFormatReaderSource(formatManager.createReaderFor(file),true);
            transportSource.setSource(readerSource);
            LOOPS = computeLoops(AUDIO_FILENAME);
            playButton->setEnabled(true);
            shiftButton->setEnabled(true);
        }
        //[/UserButtonCode_loadButton]
    }
    else if (buttonThatWasClicked == playButton)
    {
        //[UserButtonCode_playButton]
        if (Stopped == state || Paused == state) {
            changeState(Starting);
        } else if (Playing == state) changeState(Pausing);
        //[/UserButtonCode_playButton]
    }
    else if (buttonThatWasClicked == stopButton)
    {
        //[UserButtonCode_stopButton]
        if (Paused == state) {
            changeState(Stopped);
        } else
            changeState(Stopping);
        //[/UserButtonCode_stopButton]
    }
    else if (buttonThatWasClicked == settingsButton)
    {
        //[UserButtonCode_settingsButton]
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
        //[UserButtonCode_loopButton]
        if (readerSource->isLooping()) {
            readerSource->setLooping(false);
        } else
            readerSource->setLooping(true);
        //[/UserButtonCode_loopButton]
    }
    else if (buttonThatWasClicked == shiftButton)
    {
        //[UserButtonCode_shiftButton]
        if (shiftButton->getToggleState()) {
            shifting = true;
            transportSource.setPosition(drand48() * static_cast<float>(transportSource.getTotalLength()));
        }
        //[/UserButtonCode_shiftButton]
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
        //[UserSliderCode_gainSlider]
        gain = static_cast<float>(gainSlider->getValue());
        transportSource.setGain(gain);
        //[/UserSliderCode_gainSlider]
    }
    else if (sliderThatWasMoved == delaySlider)
    {
        //[UserSliderCode_delaySlider]
        //[/UserSliderCode_delaySlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] 
void AudioApp::changeListenerCallback(ChangeBroadcaster* src){

    if (&deviceManager == src) {
        AudioDeviceManager::AudioDeviceSetup setup;
        deviceManager.getAudioDeviceSetup(setup);
        setup.outputChannels.isZero() ? sourcePlayer.setSource(nullptr) :
                                        sourcePlayer.setSource(&transportSource);
    } else if (&transportSource == src){

        if (transportSource.isPlaying()) {
            Looping == state ? changeState(Looping) : changeState(Playing);
        } else {
            if (Stopping == state || Playing == state) {
                changeState(Stopped);
            } else if (Pausing == state) changeState(Paused);
        }
    }


}

void AudioApp::changeState(TransportState newState){
    if (state != newState) {
        state = newState;
        switch (state) {
            case Stopped:
                playButton->setButtonText("Play");
                stopButton->setButtonText("Stop");
                stopButton->setEnabled(false);
                transportSource.setPosition(0.0);
                break;
            case Starting:
                transportSource.start();
                break;
            case Playing:
                playButton->setButtonText("Pause");
                stopButton->setButtonText("Stop");
                stopButton->setEnabled(true);
                break;
            case Pausing:
                transportSource.stop();
                break;
            case Paused:
                playButton->setButtonText("Resume");
                stopButton->setButtonText("Return to Zero");
                break;
            case Stopping:
                transportSource.stop();
                break;
            case Looping:
                transportSource.setLooping(true);
                playButton->setButtonText("Pause");
                stopButton->setButtonText("Stop");
                stopButton->setEnabled(true);
                break;
           // case Shifting:
             //   shiftyLooping();
              //  break;
        }
    }
}

void AudioApp::shiftyLooping(){

    
}

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
  <BACKGROUND backgroundColour="ff0f0f0f">
    <ROUNDRECT pos="20 12 708 668" cornerSize="10" fill="linear: 50 50, 416 288, 0=ff262e90, 1=ff4f0080"
               hasStroke="0"/>
  </BACKGROUND>
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
  <GROUPCOMPONENT name="Buttons" id="f87700781fc32e24" memberName="mainGroup" virtualName=""
                  explicitFocusOrder="0" pos="48 104 528 392" outlinecol="66121111"
                  textcol="fff5f4f4" title="Main"/>
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
                virtualName="" explicitFocusOrder="0" pos="600 96 120 40" txtcol="ffe8d8d8"
                buttonText="Loop Sample" connectedEdges="15" needsCallback="1"
                radioGroupId="0" state="0"/>
  <SLIDER name="Gain Slider" id="9d008628b8772a4d" memberName="gainSlider"
          virtualName="" explicitFocusOrder="0" pos="592 328 112 160" thumbcol="ff7fffd4"
          trackcol="ffffffff" rotaryslideroutline="ff0000ff" min="0" max="1"
          int="0.2000000000000000111" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="2"/>
  <LABEL name="Gain Label" id="d9d519cd8dc043ff" memberName="gainLabel"
         virtualName="" explicitFocusOrder="0" pos="568 496 150 24" textCol="fff0ffff"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="ffff0000" labelText="GAIN"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Arial Black" fontsize="22.300000000000000711" bold="0"
         italic="0" justification="36"/>
  <SLIDER name="Delay Slider" id="d7c876357bbf5a7" memberName="delaySlider"
          virtualName="" explicitFocusOrder="0" pos="592 128 112 160" thumbcol="ff7fffd4"
          trackcol="ffffffff" rotaryslideroutline="ff0000ff" min="0" max="1"
          int="0.2000000000000000111" style="LinearVertical" textBoxPos="TextBoxBelow"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="2"/>
  <LABEL name="Delay Label" id="f8d9f37a8cca0c07" memberName="delayLabel"
         virtualName="" explicitFocusOrder="0" pos="576 296 150 24" textCol="fff0ffff"
         edTextCol="ff000000" edBkgCol="0" hiliteCol="ffff0000" labelText="DELAY"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Arial Black" fontsize="22.300000000000000711" bold="0"
         italic="0" justification="36"/>
  <TOGGLEBUTTON name="Shifty Button" id="717b6864ee8b9341" memberName="shiftButton"
                virtualName="" explicitFocusOrder="0" pos="472 136 72 88" txtcol="ff808080"
                buttonText="Shifty" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
