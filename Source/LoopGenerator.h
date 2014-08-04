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
#include "pool.h"
#include "essentia.h"
#include "algorithmfactory.h"
#include "ProgressWindow.h"
#include "Matrix.h"


extern const int SR;
extern const float BAR_SIZE;

struct Loop {
    float start, end;
    Loop* prev;
    Loop* next;
    std::vector<float> loopBuffer;
    essentia::Pool bin;
    essentia::Pool binStats;
  //  Matrix selfDistMat;
  //  Matrix markovChain;
};


std::vector<Loop> computeLoops(std::string audiofilename);
void createLoopPoints(const std::vector<float>& onsets, const std::vector<essentia::Real>& AUDIO_BUFFER);
void connectLoops();
float quantizeToOnset(const std::vector<float>& onsets, float value);



#endif  // LOOPGEN_H_INCLUDED
