/*
  ==============================================================================

    ProgressWindow.h
    Created: 29 Aug 2014 11:36:59am
    Author:  milrob

  ==============================================================================
*/

#ifndef PROGRESSWINDOW_H_INCLUDED
#define PROGRESSWINDOW_H_INCLUDED


/*
 ==============================================================================
 
 ProgressWindow.cpp
 Created: 25 Aug 2014 4:22:11pm
 Author:  milrob
 
 ==============================================================================
 */

#include "JuceHeader.h"

class BackgroundThread : public ThreadWithProgressWindow{
public:
    
    BackgroundThread(const int N, const std::vector<std::string> vals);
//    ~BackgroundThread();
//    BackgroundThread(const BackgroundThread& thread);
//    BackgroundThread(BackgroundThread&& thread);
//    BackgroundThread& operator=(const BackgroundThread& thread);
//    BackgroundThread& operator=(BackgroundThread&& thread);
    
    
    void run() override;
    void threadComplete(bool userPressedCancel) override;
    
private:
    const int numFeatures;
    //Need 6 status messages
    /* title, init, remaining, cleanUp, cancel, success */
    juce::StringPairArray statusMsgs;
    
};


#endif  // PROGRESSWINDOW_H_INCLUDED
