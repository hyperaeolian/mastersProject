
#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioApp.h"



/*
    put Controls and Content here
*/
class MainContentComponent   : public Component
{
public:
  
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();
    
    

private:
    AudioApp app;
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
