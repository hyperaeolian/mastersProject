/*
  ==============================================================================

    PreSynthesis.cpp
    Created: 31 Jul 2014 11:31:34am
    Author:  milrob

  ==============================================================================
*/

#include "PreSynthesis.h"

void computeSDM(std::vector<Loop> loops){
//    std::vector<essentia::Real> distVec;
//    for (int i = 0; i < loops.size(); ++i){
//        for (int j = i; j < loops.size(); ++j){
//            distVec.push_back(loops[j])
//        }
//    }
}

/*
template <typename T>
std::vector<T> createArrangedVector(std::vector<T>* sig){
    std::vector<std::vector<T> > newVec;
    std::vector<essentia::Real> beats(featureBin->value<essentia::Real>("rhythm.beats"));
    for (int i = 0; i < beats.size(); ++i) {
        for (int j = i + 1; j < beats.size(); ++j) {
            typename std::vector<T>::const_iterator x = sig->begin() + i;
            typename std::vector<T>::const_iterator y = x + j;
            std::vector<essentia::Real> tmp(x, y);
            newVec.push_back(tmp);
        }
    }
    return newVec;
}

void computeFeatsForDist(std::vector<float> *insig){
    std::vector<essentia::Real> arrVec(createArrangedVector(insig));
    for (int i = 0; i < arrVec.size(); ++i) {
    }
}

*/