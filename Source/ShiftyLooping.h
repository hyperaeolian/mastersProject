/*
  ==============================================================================

    ShiftyLooping.h
    Created: 27 Aug 2014 11:42:28am
    Author:  milrob

  ==============================================================================
*/

#ifndef SHIFTYLOOPING_H_INCLUDED
#define SHIFTYLOOPING_H_INCLUDED

#include "JuceHeader.h"
#include "LoopGenerator.h"

class ShiftyLooper :
                     public drow::AudioFilePlayerExt,
                     public drow::AudioFilePlayer::Listener,
                     public Timer


{
public:
    ShiftyLooper();
    ~ShiftyLooper(){}

    void setShifting(bool shouldShift){shifting = shouldShift;
    }
    void setMarkov(const std::vector<int>& mc){ markovChain = mc;}
    void setLoops(const std::vector<Loop>& l){ _Loops = l;}
    
    void shiftyLooping();
    
    void timerCallback() override;
    
    //drow
    void fileChanged(drow::AudioFilePlayer* player) {}
    void audioFilePlayerSettingChanged(drow::AudioFilePlayer* player, int settingCode){}
    void playerStoppedOrStarted(drow::AudioFilePlayer* player){}

private:
    bool shifting;
    std::vector<Loop> _Loops;
    std::vector<int> markovChain;
   
    
    ShiftyLooper(const ShiftyLooper&);
    ShiftyLooper(ShiftyLooper&&);
    ShiftyLooper& operator=(const ShiftyLooper&);
    ShiftyLooper& operator=(ShiftyLooper&&);
 
};


#endif  // SHIFTYLOOPING_H_INCLUDED
