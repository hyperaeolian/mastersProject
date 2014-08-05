/*
  ==============================================================================

    PreSynthesis.cpp
    Created: 31 Jul 2014 11:31:34am
    Author:  milrob

  ==============================================================================
*/

#include "PreSynthesis.h"

const char* fNames[] = {"rhythm.bpm", "rhythm.rate", "dynam.rms", "dynam.loud", "timbre.cent"};

void computeSDM(std::vector<Loop> loops){
    std::vector<std::string> features(fNames, fNames+4);
    std::vector<REAL_NUM> distVec;
    for (int i = 0; i < loops.size(); ++i){
        for (int j = i; j < loops.size(); ++j){
            int itr = 0;
            while (itr < features.size()) {
                REAL_NUM dist = euclid(loops[j].retrieve<REAL_NUM>(features[itr]),
                                       loops[i].retrieve<REAL_NUM>(features[itr]));
                std::cout << "Dist: " << dist << std::endl;
                distVec.push_back(dist); //distVec should be a Matrix
                itr++;
            }

            
        }
    }
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
*/