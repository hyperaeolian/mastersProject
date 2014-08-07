/*
  ==============================================================================

    PreSynthesis.h
    Created: 31 Jul 2014 11:31:34am
    Author:  milrob

  ==============================================================================
*/

#ifndef PRESYNTHESIS_H_INCLUDED
#define PRESYNTHESIS_H_INCLUDED


//#include "AudioApp.h"
#include "LoopGenerator.h"
#include "MATRIX.h"


//void compute_SDM(std::vector<Loop>& loops);

int overlapExists(Loop& a, Loop& b);

void computeDistances(const std::vector<Loop>& loops, MATRIX& mat);

template <typename T>
T euclid(T t1, T t2){ return sqrt(pow((t2 - t1),(T)2)); }


#endif  // PRESYNTHESIS_H_INCLUDED
