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
    shifting = forward = false;
}

ShiftyLooper::~ShiftyLooper(){}

void ShiftyLooper::audioDeviceIOCallback(const float **inputChannelData, int numInputChannels, float **outputChannelData, int numOutputChannels, int numSamples)
{
   /*
    if (shouldShiftyLoop) {
        const AudioSampleBuffer buffer(const_cast<float**>(inputChannelData), numInputChannels, numSamples);
        
        if (this->hasStreamFinished()){
            if (shifting) {
                if (forward) {
                    currentLoop = currentLoop->next;
                    this->setLoopTimes(currentLoop->prev->end, currentLoop->end);
                    this->setPosition(currentLoop->prev->end);
                } else {
                    currentLoop = currentLoop->prev;
                    this->setLoopTimes(currentLoop->next->start, currentLoop->end);
                    this->setPosition(currentLoop->next->start);
                }
            }
            
        }
        this->start();
        this->setLoopTimes(currentLoop->start, currentLoop->end);
        getNextDirection();
        updateCurrentLoop();
        
    }
    */

}

void ShiftyLooper::shiftyLooping(){
    if (this->hasStreamFinished()){
        if (shifting) {
            if (forward) {
                currentLoop = currentLoop->next;
                this->setLoopTimes(currentLoop->prev->end, currentLoop->end);
                this->setPosition(currentLoop->prev->end);
            } else {
                currentLoop = currentLoop->prev;
                this->setLoopTimes(currentLoop->next->start, currentLoop->end);
                this->setPosition(currentLoop->next->start);
            }
        }
        
        
    }
    getNextDirection();
    updateCurrentLoop();
    shiftyLooping();
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