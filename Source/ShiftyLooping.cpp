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
    for (int i = 0; i < numSamples; ++i){
        
    }

}

void ShiftyLooper::shiftyLooping(){
    
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

/*

    
    void shiftyLooping(Loop& loop){
        if (shifting) {
            if (forward) {
                //printCurrentState("Shifting Forward");
                loop = *loop.next;
                player.setLoopTimes(loop.prev->end, loop.end);
                player.setPosition(loop.prev->end);
                player.start();
                player.setLoopTimes(loop.start, loop.end);
            } else {
                loop = *loop.prev;
                if (loop.next->start >= loop.end) return;
                //printCurrentState("Shifting Backwards");
                loop = *loop.prev;
                player.setLoopTimes(loop.next->start, loop.end);
                player.setPosition(loop.next->start);
                player.start();
                player.setLoopTimes(loop.start, loop.end);
            }
        } else {
            player.setLoopTimes(loop.start, loop.end);
            player.setPosition(loop.start);
            player.setLoopBetweenTimes(true);
        }
        
        pivot(currentLoop);
        
    }
    

*/