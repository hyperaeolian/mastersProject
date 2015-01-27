/*
  ==============================================================================

    ShiftyLooping.cpp
    Created: 23 Aug 2014 12:30:11am
    Author:  milrob

  ==============================================================================
*/

#include "ShiftyLooping.h"

ShiftyLooper::ShiftyLooper(){
    shifting = false;
}

void ShiftyLooper::shiftyLooping(){
    static int itr = 1;
    Loop curr = _Loops[itr];
    if (shifting){
        this->setLoopBetweenTimes(false);
        this->stop();
        Loop old = _Loops[markovChain[itr-1]];
        curr = _Loops[markovChain[itr]];
        if (old.start > curr.start){
            this->setLoopTimes(curr.start, curr.end);
            this->setNextPositionOverride(old.start * 44100);
        } else {
            this->setLoopTimes(curr.start, curr.end);
            this->setNextPositionOverride(old.start * 44100);
        }
        this->setLoopBetweenTimes(true);
        this->start();
        int interval = (curr.end - curr.start)*1000;
        startTimer(interval + interval * 0.5);
        if (itr+1 > markovChain.size()){
            shifting = false;
        } else {
            itr++;
        }
    } else
        return;


}

void ShiftyLooper::timerCallback(){
    shifting = !(shifting);
    shiftyLooping();
}


