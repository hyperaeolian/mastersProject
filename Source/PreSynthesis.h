/*
  ==============================================================================

    PreSynthesis.h
    Created: 31 Jul 2014 11:31:34am
    Author:  milrob

  ==============================================================================
*/

#ifndef PRESYNTHESIS_H_INCLUDED
#define PRESYNTHESIS_H_INCLUDED

#include <vector>
#include "FeatureExtractor.h"


template <typename T>
std::vector<T> createArrangedVector(std::vector<float>* sig);

void computeFeatsForDist(std::vector<float> *insig);


#endif  // PRESYNTHESIS_H_INCLUDED
