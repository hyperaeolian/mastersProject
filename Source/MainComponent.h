
#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioApp.h"



/*
    put Controls and Content here
*/
class MainContentComponent   : public Component, public MenuBarModel
{
public:
  
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();
    
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex(int index, const String& name) override;
    void menuItemSelected(int menuID, int index) override;
    
    enum MenuIDs {
        LabelClear = 1000,
        Open,
        Settings
    };
    
    

private:
    AudioApp app;
    MenuBarComponent menuBar;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
