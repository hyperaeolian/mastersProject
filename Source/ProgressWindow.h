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
#include "AudioApp.h"


class BackgroundThread : public ThreadWithProgressWindow{
public:
    
    BackgroundThread(const int N) : ThreadWithProgressWindow("Exracting features...", true, true),
                                    numFeatures(N)
    {
        setStatusMessage("Initializing Feature Extractor");
    }
    
    void run() override{
        setProgress (-0.5);
        setStatusMessage ("Preparing file for Feature Extraction");
        wait (2000);
        //const int features = 25;
        
        for (int i = 0; i < numFeatures; ++i){
            if (threadShouldExit()) return;
            setProgress (i / (double) numFeatures);
            setStatusMessage (String (numFeatures - i) + " features left to extract...");
            wait (500);
        }
        
        setProgress (-0.5);
        setStatusMessage ("Cleaning Up");
       // wait (2000);
    }
    
    void threadComplete(bool userPressedCancel) override {
        if (userPressedCancel) {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Progress Window", "Cancelling feature extraction");
        } else {
            setStatusMessage("Extraction Successful!");
            AlertWindow::showMessageBoxAsync(AlertWindow::NoIcon, "Progress Window", "Feature Extraction Successful!");
        }
    }
private:
    const int numFeatures;
    
};



#endif  // PROGRESSWINDOW_H_INCLUDED
