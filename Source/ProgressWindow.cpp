/*
  ==============================================================================

    ProgressWindow.cpp
    Created: 25 Aug 2014 4:22:11pm
    Author:  milrob

  ==============================================================================
*/

#include "ProgressWindow.h"

BackgroundThread::BackgroundThread(const int N, const std::vector<std::string> vals) :
     ThreadWithProgressWindow("Progress", true, true), numFeatures(N)
    {
        std::vector<std::string> keys = {"title", "init","remaining", "cleanUp", "cancel", "success" };
        for (int i = 0; i < keys.size(); ++i){
            statusMsgs.set(keys[i], vals[i]);
        }
        setStatusMessage(statusMsgs["init"]);

    }
    
void BackgroundThread::run(){
        setProgress (-0.5);
        //setStatusMessage (statusMsgs["init"]);
        wait (2000);
        for (int i = 0; i < numFeatures; ++i){
            if (threadShouldExit()) return;
            setProgress (i / (double) numFeatures);
            setStatusMessage (String (numFeatures - i) + statusMsgs["remaining"]);
            wait (500);
        }
        
        setProgress (-0.5);
        setStatusMessage (statusMsgs["cleanUp"]);
    }
    
void BackgroundThread::threadComplete(bool userPressedCancel){
        if (userPressedCancel) {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Progress Window", statusMsgs["cancel"]);
        } else {
            setStatusMessage(statusMsgs["success"]);
            AlertWindow::showMessageBoxAsync(AlertWindow::NoIcon, "Progress Window", "Feature Extraction Successful!");
        }
    }
