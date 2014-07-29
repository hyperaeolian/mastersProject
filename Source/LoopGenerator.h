/*
  ==============================================================================

    LoopGen.h
    Created: 16 Jul 2014 2:23:14pm
    Author:  milrob

  ==============================================================================
*/

#ifndef LOOPGEN_H_INCLUDED
#define LOOPGEN_H_INCLUDED

//#include <iostream>
#include <vector>
#include "JuceHeader.h"

extern const int SR;
extern const float BAR_SIZE;
    
struct Loop {
    float start, end;
    Loop* prev;
    Loop* next;
    //MATRIX markovChain;
};


std::vector<Loop> computeLoops(std::string audiofilename);
void createLoopPoints(const std::vector<float>& onsets);
void connectLoops();
float quantizeToOnset(const std::vector<float>& onsets, float value);
//template <typename T> T retrieveFeature(T featType, std::string featName);



#endif  // LOOPGEN_H_INCLUDED
