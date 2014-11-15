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


/* ===========================================================================*//*
|               [List of features]                                              |
|                                                                               |
| Rhythm:                                                                       |
|    Tempogram, onsetTimes, Beats, beatIntervals                                |
| ----------------------------------------------------------------------------- |
| Dynamics:                                                                     |
|    dynamicRangeCoeff, RMS, Loudness                                           |
| ----------------------------------------------------------------------------- |
| Timbre:                                                                        |
|    mfccBands, mfccCoeffs, centroid                                            |
| ----------------------------------------------------------------------------- |
| Tonal:                                                                        |
|    Chords:                                                                    |
|        changeRate, numberRate, key, scale, histogram, strength, progression   |
|    Key:                                                                       |
|        scale, key                                                             |
|    Pitch:                                                                     |
|        hpcp, hpcpHighRes                                                      |
|                                                                               |
*//*=========================================================================== */


using namespace essentia;
using namespace essentia::standard;


class FeatureExtractor {
public:
    explicit FeatureExtractor(const VEC_REAL& buffer);
    ~FeatureExtractor();
    
    //allow for alternate delimiters
    //virtual void findDelimiters() = 0;
    
    void findOnsets();
    void findBeats();
    void computeFeaturesForLoop(Loop& loop);
    
    Real getTempo()               const { return bpm; }
    VEC_REAL getOnsets() const { return onsets; }
    VEC_REAL getBeats()  const { return beats; }
    
private:

    const VEC_REAL AudioBuffer;
    VEC_REAL onsets, beats;
    const int SR, FrameSize, HopSize;
    bool successfulExtraction;
    Real bpm;
    
    FeatureExtractor();
    FeatureExtractor(const FeatureExtractor&);
    FeatureExtractor(FeatureExtractor&&);
    FeatureExtractor& operator=(const FeatureExtractor&);
    FeatureExtractor& operator=(FeatureExtractor&&);
};
    

#endif  // FEATUREEXTRACTOR_H_INCLUDED
