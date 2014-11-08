/*
  ==============================================================================

    ShiftyLooping.cpp
    Created: 23 Aug 2014 12:30:11am
    Author:  milrob

  ==============================================================================
*/

#include "ShiftyLooping.h"


ShiftyLooper::ShiftyLooper(){
    index = 0;
    shifting = forward = shouldShiftyLoop = false;
}

ShiftyLooper::~ShiftyLooper(){}

void ShiftyLooper::audioDeviceIOCallback(const float **inputChannelData, int numInputChannels, float **outputChannelData, int numOutputChannels, int numSamples)
{

    if (shouldShiftyLoop){
        
    }

}

void ShiftyLooper::shiftyLooping(){
    shouldShiftyLoop = true;
    
    /*
    if (hasStreamFinished()){
        getNextDirection();
        updateCurrentLoop();
        if (shifting) {
            if (forward) {
                currentLoop->prev = &_Loops[markovChain[index-1]];
                this->setLoopTimes(currentLoop->prev->end, currentLoop->end);
                this->setPosition(currentLoop->prev->end);
            } else { //going backwards
                currentLoop->next = &_Loops[markovChain[index+1]];
                this->setLoopTimes(currentLoop->next->start, currentLoop->end);
                this->setPosition(currentLoop->next->start);
            }
        }
        
    }
    this->start();
    this->setLoopTimes(currentLoop->start, currentLoop->end);
    setNextReadPosition(currentLoop->start * 44100);
     */

}

void ShiftyLooper::getNextDirection(){
    if (markovChain[index+1] > markovChain[index]) {
        shifting = forward = true;
    } else if (markovChain[index+1] < markovChain[index]) {
        shifting = true;
        forward = false;
    } else {
        shifting = false;
    }
    
}

inline void ShiftyLooper::updateCurrentLoop(){
    if (index++ >= markovChain.size()-1) {
        updateMarkov = true;
        index++;
    } else
        index++;
    currentLoop = &_Loops[markovChain[index]];
}
