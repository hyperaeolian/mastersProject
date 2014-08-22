
#include "MainComponent.h"


MainContentComponent::MainContentComponent() : menuBar(this){
    addAndMakeVisible(&app);
    addAndMakeVisible(&menuBar);
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
    menuBar.setBounds(0, 0, getWidth(), 20);
    app.setBounds(0, 0, getWidth(), getHeight());
}

StringArray MainContentComponent::getMenuBarNames(){
    const char* menuNames[] = {"File", "Edit", 0};
    return StringArray(menuNames);
}

PopupMenu MainContentComponent::getMenuForIndex(int index, const juce::String &name){
    PopupMenu menu;
    if (name == "File"){
        menu.addItem(LabelClear, "Clear");
        menu.addItem(Open, "Open...");
        menu.addItem(Settings, "Audio Settings");
    } else if (name == "Edit") {
        //menu.addItem();
    }
    return menu;
}

void MainContentComponent::menuItemSelected(int menuID, int index){
    switch (menuID) {
        case LabelClear:
            std::cout << "Label Clear" << std::endl;
            break;
        case Open:
            app.loadFile();
            break;
        case Settings:
            app.openAudioSettings();
            break;
        default:
            break;
    }
}