/*
  ==============================================================================

    PreSynthesis.cpp
    Created: 31 Jul 2014 11:31:34am
    Author:  milrob

  ==============================================================================
*/

#include "PreSynthesis.h"

const int numFeatures = 4;
const char* fNames[] = {"dynam.rms", "timbre.cent", "tonal.keyStr", "dynam.dyRange"};

int overlapExists(const Loop& a, const Loop& b){
    if (a.start < b.end || a.end > b.start) {
        return false;
    } else {
        return true;
    }
}

inline void initalize_matrix(const std::vector<Loop>& loops, MATRIX& mat){
    for (int i = 0; i < mat.rows(); ++i){
        for (int j = i + 1; j < mat.cols(); ++j){
            if (i == j){
                mat(i,j) = 0;
                continue;
            } else {
                mat(i,j) = overlapExists(loops.at(i), loops.at(j));
                mat(j,i) = mat(i,j);
            }
        }
    }
}

void computeDistance(const std::vector<Loop>& loops, MATRIX& mat){
    initalize_matrix(loops, mat);
    
    std::vector<std::string> feature_names(fNames, fNames + numFeatures);
    for (int i = 0; i < mat.rows(); ++i) {
        for (int j = 0; j < mat.cols(); ++j) {
            if (mat(i,j) == 0 || i == j) {
                continue;
            } else {
                essentia::Real dist = 0.0;
                for (int k = 0; k < feature_names.size(); ++k){
                    dist += euclidean(loops.at(i).bin.value<essentia::Real>(feature_names[k]),
                                      loops.at(j).bin.value<essentia::Real>(feature_names[k]));
                }
                mat(i,j) = dist;
                mat(j,i) = mat(i,j);
            }
        }
    }    
}
