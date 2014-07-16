/*
  ==============================================================================

    LoopGenerator.h
    Created: 16 Jul 2014 11:12:55am
    Author:  milrob

  ==============================================================================
*/

#ifndef LOOPGENERATOR_H_INCLUDED
#define LOOPGENERATOR_H_INCLUDED

#include <vector>

using namespace std;

const int SR = 44100;
const float BAR_SIZE = 3.0;

struct Loop {
    int start, end;
    Loop* prev;
    Loop* next;
};


vector<Loop> LOOP_REPO;
extern float LAST_ONSET;

vector<Loop> computeLoops(const std::string audiofilename);
void createLoops(const vector<float>& onsets);
void connectLoops();
float quantizeToOnset(const vector<float>& onsets, float value);

#endif  // LOOPGENERATOR_H_INCLUDED
