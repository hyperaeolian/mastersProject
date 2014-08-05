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
#include "AudioApp.h" /* Why do I need this include? */
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


extern const int FRAME_SIZE, HOP, NumFeatures;
extern bool successfulExtraction, onlyRhythm;
extern essentia::Pool* featureBin;


void computeFeaturesForLoop(Loop& loop);
std::vector<VAR> computeGlobalBeatsOnsets(const std::string song);



#endif  // FEATUREEXTRACTOR_H_INCLUDED
