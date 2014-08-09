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



int overlapExists(const Loop& a, const Loop& b);

void initialize_matrix(const std::vector<Loop>& loops, MATRIX& mat);

void computeDistances(const std::vector<Loop>& loops, MATRIX& mat);

MATRIX computeTransitionMatrix(const MATRIX& mat);

std::vector<std::pair<int, int> > markov(const MATRIX& transMat, const int num_iters, int state);

template <typename T>
T euclidean(T t1, T t2){ return sqrt(pow((t2 - t1),(T)2)); }


#endif  // PRESYNTHESIS_H_INCLUDED
