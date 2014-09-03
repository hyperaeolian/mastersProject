/*
  ==============================================================================

    PreSynthesis.h
    Created: 31 Jul 2014 11:31:34am
    Author:  milrob

  ==============================================================================
*/

#ifndef PRESYNTHESIS_H_INCLUDED
#define PRESYNTHESIS_H_INCLUDED


#include "LoopGenerator.h"
#include "MATRIX.h"


namespace mkov{

class MarkovChain {
public:
    MarkovChain(const std::vector<Loop>& _loops, MATRIX& inMat, int r, int c);
    ~MarkovChain();
    
    void computeDistances();
    MATRIX computeTransitionMatrix();
    std::vector<int> markov(const MATRIX& transMat, int iters, int state);
    
    template <typename T>
    T euclidean(T t1, T t2){ return sqrt((t2-t1)*(t2-t1)); }
    
private:
    int rows, cols;
    const std::vector<Loop> _Loops;
    MATRIX inputMatrix, transitionMatrix;
    const int numFeatures = 4;
    
    int overlapExists(const Loop& a, const Loop& b);
    void initMatrix();
};

    std::vector<int> generateMarkovChain(const std::vector<Loop>& loops, int itr, int start);
    
}
#endif  // PRESYNTHESIS_H_INCLUDED
