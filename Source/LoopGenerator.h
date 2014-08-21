/*
  ==============================================================================

    LoopGen.h
    Created: 16 Jul 2014 2:23:14pm
    Author:  milrob

  ==============================================================================
*/

#ifndef LOOPGEN_H_INCLUDED
#define LOOPGEN_H_INCLUDED

#include <vector>
#include "JuceHeader.h"
#include "pool.h"
#include "essentia.h"
#include "algorithmfactory.h"
#include "ProgressWindow.h"



extern const int SR;
extern const float BAR_SIZE;


struct Loop {
    float start, end;
    int head, tail;
    int numBeats;
    Loop* prev;
    Loop* next;
    essentia::Pool bin;
    essentia::Pool binStats;
};


std::vector<Loop> computeLoops(const std::string audiofilename, essentia::Real& bpm);
void createLoopPoints(const std::vector<float>& onsets, const std::vector<essentia::Real>& AUDIO_BUFFER, std::vector<Loop>& loops);
void connectLoops(std::vector<Loop>& loops);
void findOverlaps(std::vector<Loop>& loops);
float quantizeToOnset(const std::vector<float>& onsets, float value);
void printBuffer(Loop& a);



#endif  // LOOPGEN_H_INCLUDED
