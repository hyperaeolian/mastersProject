/*
  ==============================================================================

    PreSynthesis.cpp
    Created: 31 Jul 2014 11:31:34am
    Author:  milrob

  ==============================================================================
*/

#include "MarkovChain.h"

namespace mkov{

//These are currently the features being used to calculate similarity
const char* features[] = {"dynam.rms", "timbre.cent", "tonal.keyStr", "dynam.dyRange"};

MarkovChain::MarkovChain(const std::vector<Loop>& _loops, MATRIX& inMat, int r, int c) :
    rows(r), cols(c), _Loops(_loops),inputMatrix(inMat)
{
    initMatrix();
}

MarkovChain::~MarkovChain(){}

inline int MarkovChain::overlapExists(const Loop& a, const Loop& b){
    if (a.start < b.end || a.end > b.start) {
        return false;
    } else {
        return true;
    }
}

void MarkovChain::initMatrix() {
    for (int i = 0; i < inputMatrix.rows(); ++i){
        for (int j = i + 1; j < inputMatrix.cols(); ++j){
            if (i == j){
                inputMatrix(i,j) = 1;
                continue;
            } else {
                inputMatrix(i,j) = overlapExists(_Loops[i], _Loops[j]);
                inputMatrix(j,i) = inputMatrix(i,j);
            }
        }
    }
}

void MarkovChain::computeDistances(){
   // assert(loops.size() * loops.size() == mat.cols() * mat.rows());
    
    std::vector<std::string> feature_names(features, features + numFeatures);
    for (int i = 0; i < inputMatrix.rows(); ++i) {
        for (int j = i + 1; j < inputMatrix.cols(); ++j) {
            //if loop doesn't overlap or is on the diagnol, skip it
            if (inputMatrix(i,j) == 0 || i == j){
                continue;
            } else {
                _REAL dist = 0.f;
                for (int k = 0; k < feature_names.size(); ++k){
                    dist += euclidean(_Loops.at(i).bin.value<essentia::Real>(feature_names[k]),
                                      _Loops.at(j).bin.value<essentia::Real>(feature_names[k]));
                }
                inputMatrix(i,j) = dist;
                inputMatrix(j,i) = inputMatrix(i,j);
            }
        }
    }
}

MATRIX MarkovChain::computeTransitionMatrix(){
    MATRIX transitions(inputMatrix);
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


std::vector<int> MarkovChain::markov(const MATRIX& transMat, int num_iters, int state){
    juce::Random r;
    std::vector<int> chain;
    
    for (int n = 0; n < num_iters - 1; ++n){
        float probability = r.nextFloat();
        for (int i = 0; i < transMat.cols(); ++i){
            probability -= transMat(state,i);
            if (probability < 0) {
                break;
            }
            chain.push_back(i);
        }
    }
    return chain;
}

//namespace (non-member) convenience function
    
    std::vector<int> generateMarkovChain(const std::vector<Loop>& loops, int itr, int start){
        std::vector<int> chain;
        
        std::vector<std::string> vals = {"Foo", "Preparing for Analysis", " distances to calculate",
            "Finding similarity", "You canceled the similarity calculation",
            "Similary Metrics Complete!"};
        
        BackgroundThread simThread(loops.size(), vals);
        if (simThread.runThread()){
            int row = loops.size(), col = loops.size();
            MATRIX mat(row, col);
            MarkovChain markovChain(loops, mat, row, col);
            markovChain.computeDistances();
            MATRIX transMatrix(markovChain.computeTransitionMatrix());
            chain = markovChain.markov(transMatrix, itr, start);
        } else
            simThread.threadComplete(true);
        
        return chain;
    }
    
    
    
}