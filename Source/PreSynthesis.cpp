/*
  ==============================================================================

    PreSynthesis.cpp
    Created: 31 Jul 2014 11:31:34am
    Author:  milrob

  ==============================================================================
*/

#include "PreSynthesis.h"


const char* fNames[] = {"rhythm.bpm", "rhythm.rate", "dynam.rms", "dynam.loud", "timbre.cent"};

int overlapExists(Loop a, Loop b){
    if (a.start < b.end || a.end > b.start) {
        return false;
    } else {
        return true;
    }
}

void computeDistance(const std::vector<Loop>& loops, MATRIX& mat){
    assert(mat.rows() <= loops.size());
    for (int i = 0; i < mat.rows(); ++i){
        for (int j = i + 1; j < mat.cols(); ++j){
            if (i == j){
                mat(i,j) = 0;
                continue;
            } else {
                mat(i,j) = overlapExists(loops[i], loops[j]);
                mat(j,i) = mat(i,j);
            }
        }
    }
    
    for (int i = 0; i < mat.rows(); ++i) {
        for (int j = 0; j < mat.cols(); ++j) {
            if (mat(i,j) == 0) {
                continue;
            } else {
                mat(i,j) = euclid(loops[i].bin.value<essentia::Real>("dynam.rms"),
                                  loops[j].bin.value<essentia::Real>("dynam.rms"));
            }
        }
    }    
}
