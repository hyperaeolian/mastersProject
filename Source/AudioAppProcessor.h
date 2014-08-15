/*
  ==============================================================================

    AudioProcessor.h
    Created: 14 Aug 2014 3:43:47pm
    Author:  milrob

  ==============================================================================
*/

#ifndef AUDIOPROCESSOR_H_INCLUDED
#define AUDIOPROCESSOR_H_INCLUDED

#include "AudioApp.h"

/*
class ShiftyLooping : public ChangeBroadcaster, public MessageListener, public Timer {
public:
    
    ShiftyLooping(std::vector<int>& sequence, Loop& current);
    ShiftyLooping(ShiftyLooping& sl);
    ShiftyLooping& operator=(ShiftyLooping&& sl);
    ~ShiftyLooping();
    
    void startLoop();
    void handleMessage(const Message& message) override;
    
    
    
private:
    ShiftyLooping(); //Don't want this ctor used
    Loop loop;
    bool shifting, forward;
    
    void timerCallback() override;
};

*/

#endif  // AUDIOPROCESSOR_H_INCLUDED
