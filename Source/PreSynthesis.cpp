/*
  ==============================================================================

    PreSynthesis.cpp
    Created: 31 Jul 2014 11:31:34am
    Author:  milrob

  ==============================================================================
*/

#include "PreSynthesis.h"

const char* fNames[] = {"rhythm.bpm", "rhythm.rate", "dynam.rms", "dynam.loud", "timbre.cent"};

//Compute a self distance matrix of the loops using the fNames features
void compute_SDM(std::vector<Loop>& loops){
    std::vector<std::string> features(fNames, fNames+4);
    Matrix<VAR> sdm(loops.size(), loops.size());
    for (int i = 0; i < loops.size(); ++i){
        for (int j = i; j < loops.size(); ++j){
            int itr = 0;
            while (itr < features.size()) {
                VAR dist = euclid(loops[j].retrieve<VAR>(features[itr]),
                                       loops[i].retrieve<VAR>(features[itr]));
                sdm[i][j] = dist;
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