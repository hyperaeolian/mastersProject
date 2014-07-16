/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent(){
    addAndMakeVisible(&app);
    setSize (app.getWidth(), app.getHeight());
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xffeeddff));

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    app.setBounds(0, 0, getWidth(), getHeight());
}
