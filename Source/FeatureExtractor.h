/*
  ==============================================================================

    FeatureExtractor.h
    Created: 28 Jul 2014 1:53:06pm
    Author:  milrob

  ==============================================================================
*/

#ifndef FEATUREEXTRACTOR_H_INCLUDED
#define FEATUREEXTRACTOR_H_INCLUDED

#include "JuceHeader.h"
#include "essentia.h"
#include "essentiamath.h"
#include "LoopGenerator.h"
#include "pool.h"
#include "algorithmfactory.h"



using namespace essentia;
using namespace essentia::standard;


class FeatureExtractor {
public:
    explicit FeatureExtractor(const VEC_REAL& buffer);
    ~FeatureExtractor();
    
    //allow for alternate delimiters
    //virtual void findDelimiters();
    
    //void findOnsets();
   // void findBeats();
    void computeFeaturesForBuffer();
    void computeFeaturesForLoop(Loop& loop);
    
    VEC_REAL getOnsets() const { return onsets; }
    VEC_REAL getBeats()  const { return beats; }
    
private:

    const VEC_REAL AudioBuffer;
    VEC_REAL onsets, beats;
    const int SR, FrameSize, HopSize;
    bool successfulExtraction;
    
    FeatureExtractor();
    FeatureExtractor(const FeatureExtractor&);
    FeatureExtractor(FeatureExtractor&&);
    FeatureExtractor& operator=(const FeatureExtractor&);
    FeatureExtractor& operator=(FeatureExtractor&&);
};
    

#endif  // FEATUREEXTRACTOR_H_INCLUDED
