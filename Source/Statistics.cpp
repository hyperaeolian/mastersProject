/*
  ==============================================================================

    PreSynthesis.cpp
    Created: 31 Jul 2014 11:31:34am
    Author:  milrob

  ==============================================================================
*/

#include "Statistics.h"

MarkovChainGen::MarkovChainGen(int r, int c, std::vector<Loop>& _loops, MATRIX& inMat) :
    rows(r), cols(c), loops(_loops),inputMatrix(inMat)
{
    initMatrix(loops, inputMatrix);
}

MarkovChainGen::~MarkovChainGen(){}

inline int MarkovChainGen::overlapExists(const Loop& a, const Loop& b){
    if (a.start < b.end || a.end > b.start) {
        return false;
    } else {
        return true;
    }
}

inline void MarkovChainGen::initMatrix(const std::vector<Loop> &loops, MATRIX &mat) {
    for (int i = 0; i < mat.rows(); ++i){
        for (int j = i + 1; j < mat.cols(); ++j){
            if (i == j){
                mat(i,j) = 1;
                continue;
            } else {
                mat(i,j) = overlapExists(loops.at(i), loops.at(j));
                mat(j,i) = mat(i,j);
            }
        }
    }
}

void MarkovChainGen::computeDistances(const std::vector<Loop>& loops, MATRIX& mat){
    assert(loops.size() * loops.size() == mat.cols() * mat.rows());
    
    std::vector<std::string> feature_names(fNames, fNames + numFeatures);
    for (int i = 0; i < mat.rows(); ++i) {
        for (int j = i + 1; j < mat.cols(); ++j) {
            //if loop doesn't overlap or is on the diagnol, skip it
            if (mat(i,j) == 0 || i == j){
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

MATRIX MarkovChainGen::computeTransitionMatrix(const MATRIX& mat){
    MATRIX transitions(mat);
    for (int i = 0; i < transitions.rows(); ++i){
        essentia::Real rowSum = 0.f;
        for (int j = 0; j < 2 * transitions.cols(); ++j){
            if (j < transitions.cols()){
                rowSum += transitions(i,j);
            } else {
                int k = j % transitions.cols();
                transitions(i,k) /= rowSum;
            }
        }
    }
    return transitions;
}


std::vector<essentia::Real> MarkovChainGen::markovChain(const MATRIX& transMat, const int num_iters, int state){
    juce::Random r;
    std::vector<essentia::Real> chain;
    
    for (int n = 0; n < num_iters - 1; ++n){
        float probability = r.nextFloat();
        for (int i = 0; i < transMat.cols(); ++i){
            probability -= transMat(state,i);
            if (probability < 0) {
                break;
            }
            chain.push_back(static_cast<int>(i));
        }
    }
    return chain;
}


/*
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
                mat(i,j) = 1;
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
        for (int j = i + 1; j < mat.cols(); ++j) {
            //if loop doesn't overlap or is on the diagnol, skip it
            if (mat(i,j) == 0 || i == j){
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

MATRIX computeTransitionMatrix(const MATRIX& mat){
    MATRIX transitions(mat);
    for (int i = 0; i < transitions.rows(); ++i){
        essentia::Real rowSum = 0.f;
        for (int j = 0; j < 2 * transitions.cols(); ++j){
            if (j < transitions.cols()){
                rowSum += transitions(i,j);
            } else {
                int k = j % transitions.cols();
                transitions(i,k) /= rowSum;
            }
        }
    }
    return transitions;
}


std::vector<essentia::Real> markov(const MATRIX& transMat, const int num_iters, int state){
    juce::Random r;
    std::vector<essentia::Real> chain;
    
    for (int n = 0; n < num_iters - 1; ++n){
        float probability = r.nextFloat();
        for (int i = 0; i < transMat.cols(); ++i){
            probability -= transMat(state,i);
            if (probability < 0) {
                break;
            }
            chain.push_back(static_cast<int>(i));
        }
    }
    return chain;
}
*/

