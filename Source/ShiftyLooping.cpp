/*
  ==============================================================================

    ShiftyLooping.cpp
    Created: 23 Aug 2014 12:30:11am
    Author:  milrob

  ==============================================================================
*/

#include "ShiftyLooping.h"


void ShiftyLooper::shiftyLooping(Loop& curr, Loop& prev, bool isShifting, bool forward){
    
    /*
     
     if one or more Loops exist between curr and prev
     for each loop from prev to curr
        set playback position to intermediary loop
        play loop
        set next playback position
     
     */
    
    
    if (isShifting){
        this->setLoopBetweenTimes(false);
        this->stop();
        if (prev.start > curr.start){
            this->setLoopTimes(curr.start, curr.end);
            this->setNextPositionOverride(prev.start * 44100);
        } else {
            this->setLoopTimes(curr.start, curr.end);
            this->setNextPositionOverride(prev.start * 44100);
        }
        this->setLoopBetweenTimes(true);
        this->start();
        int interval = (curr.end - curr.start)*1000;
        isShifting = false;
    }
}


