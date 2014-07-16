
#include "MainComponent.h"


MainContentComponent::MainContentComponent(){
    addAndMakeVisible(&app);
    setSize (app.getWidth(), app.getHeight());
}

MainContentComponent::~MainContentComponent(){}

void MainContentComponent::paint (Graphics& g){
    g.fillAll (Colour (0xffeeddff));

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized(){
    app.setBounds(0, 0, getWidth(), getHeight());
}
