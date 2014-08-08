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

void computeDistances(const std::vector<Loop>& loops, MATRIX& mat){
    assert(loops.size() * loops.size() == mat.cols() * mat.rows());
    initalize_matrix(loops, mat);
    
    std::vector<std::string> feature_names(fNames, fNames + numFeatures);
    for (int i = 0; i < mat.rows(); ++i) {
        for (int j = 0; j < mat.cols(); ++j) {
            if (mat(i,j) == 0 || i == j) {
                continue;
            } else {
                essentia::Real dist = 0.f;
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

MATRIX markovizeDistanceMatrix(const MATRIX& mat){
    MATRIX markov(mat);
    std::cout << "MARKOV BEFORE: " << markov << std::endl;
    for (int i = 0; i < markov.rows(); ++i){
        essentia::Real rowSum = 0.f;
        std::cout << std::endl;
        for (int j = 0; j < 2 * markov.cols(); ++j){
            if (j < markov.cols()){
                rowSum += markov(i,j);
            } else {
                int k = j % markov.cols();
                markov(i,k) /= rowSum;
            }
            //j < markov.cols()  ?  rowSum += markov(i,j)  :  markov(i,(j/2)) /= rowSum;
        }
    }
    std::cout << "MARKOV AFTER: " << markov << std::endl;
    return markov;
}


std::vector<essentia::Real> markov_chain(const MATRIX& mc, int max_iters, int startingPos){
    /* Untested */
    int current = startingPos;
    juce::Random r;
    std::vector<essentia::Real> chain;
    chain.push_back(startingPos);
    for (int n = 0; n < max_iters - 1; ++n){
        essentia::Real probability = r.nextFloat();
        for (int i = 0; i < mc.rows(); ++i){
            probability -= mc(current,i);
            if (probability < 0) {
                break;
            }
            chain.push_back(i);
        }
    }
    return chain;
}
