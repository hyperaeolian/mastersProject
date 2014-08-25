
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
    const char* menuNames[] = {"File", "Options", 0};
    return StringArray(menuNames);
}

PopupMenu MainContentComponent::getMenuForIndex(int index, const juce::String &name){
    PopupMenu menu;
   if (name == "File"){
        menu.addItem(LabelClear, "Clear");
        menu.addItem(Open, "Open...");
        menu.addItem(Save, "Save");
    } else if (name == "Options") {
        menu.addItem(Options, "View Loop List");
        menu.addItem(Settings, "Audio Settings");
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
        case Save:
            AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon, "Save File", "Saving File");
        case Settings:
            app.openAudioSettings();
            break;
        case Options:
            app.isTableEnabled() ? app.showLoopTable() :
                AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,
                                                 "Table Unavailable", "You must load a sound file first");
            break;
        default:
            break;
    }
}