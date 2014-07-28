/*
  ==============================================================================

    ProgressWindow.h
    Created: 28 Jul 2014 1:45:18pm
    Author:  milrob

  ==============================================================================
*/

#ifndef PROGRESSWINDOW_H_INCLUDED
#define PROGRESSWINDOW_H_INCLUDED

#include "JuceHeader.h"


class BackgroundThread : public ThreadWithProgressWindow{
public:
    
    BackgroundThread() : ThreadWithProgressWindow("Exracting features...", true, true){
        setStatusMessage("Initializing Feature Extractor");
    }
    
    void run() override{
        setProgress (-1.0);
        setStatusMessage ("Preparing file for Feature Extraction");
        wait (2000);
        const int features = 25;
        
        for (int i = 0; i < features; ++i){
            if (threadShouldExit()) return;
            setProgress (i / (double) features);
            setStatusMessage (String (features - i) + " features left to extract...");
            wait (500);
        }
        
        setProgress (-1.0);
        setStatusMessage ("Cleaning Up");
        wait (2000);
    }
    
    void threadComplete(bool userPressedCancel) override {
        if (userPressedCancel) {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Progress Window", "Cancelling feature extraction");
        } else {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Progress Window", "Feature Extraction Successful!");
        }
    }
    
};



#endif  // PROGRESSWINDOW_H_INCLUDED
