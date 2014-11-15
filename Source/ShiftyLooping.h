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

class ShiftyLooper : public drow::AudioFilePlayerExt,
                     public drow::AudioFilePlayer::Listener


{
public:
    ShiftyLooper(){}
    ~ShiftyLooper(){}

    void shiftyLooping(bool forward, double init, double start, double end){
        assert(init < end && start < end);
        if (forward){
            /* PUT THIS IS AUDIOAPP TRANSPORTSTATE */
//            this->setLoopTimes(start, end);
//            this->setNextReadPosition(int64(init));
//            this->setLoopBetweenTimes(true);
    
        } 
    }
    
    
    
    //drow
    void fileChanged(drow::AudioFilePlayer* player) {}
    void audioFilePlayerSettingChanged(drow::AudioFilePlayer* player, int settingCode){}
    void playerStoppedOrStarted(drow::AudioFilePlayer* player){}

private:
    
    ShiftyLooper(const ShiftyLooper&);
    ShiftyLooper(ShiftyLooper&&);
    ShiftyLooper& operator=(const ShiftyLooper&);
    ShiftyLooper& operator=(ShiftyLooper&&);
 
};


#endif  // SHIFTYLOOPING_H_INCLUDED
