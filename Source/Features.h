/*
  ==============================================================================

    Features.h
    Created: 24 Jul 2014 2:57:58pm
    Author:  milrob

  ==============================================================================
*/

#ifndef FEATURES_H_INCLUDED
#define FEATURES_H_INCLUDED

#include "essentia.h"
#include "essentiamath.h"
#include "pool.h"
#include "algorithmfactory.h"

using namespace essentia;
using namespace essentia::standard;

namespace fext {
    
    //Utilities
    const int SR         = 44100;
    const int FRAME_SIZE = 2048;
    const int HOP        = FRAME_SIZE / 2;
    std::string wintype  = "hann";
    Pool featureBin;
    std::vector<Real> audioBuffer, frame, windowedFrame, spectrum;
    
    
    //Rhythm Features
    Real bpm, onsetRate, beatsConfidence;
    std::vector<Real> tempogram, onsetTimes, beats, beatIntervals;
    
    //Dynamics Features
    Real dynamicRangeCoeff;
    std::vector<Real> rms, loudness;
    
    //Timbral Features
    std::vector<Real> mfccBands, mfccCoeffs, centroid;
    
    //Pitch Features
    Real                            chords_ChangeRate, chords_NumberRate;
    Real                            key_Strength;
    std::string                     chords_key, chords_Scale;
    std::string                     key_Key, key_Scale;
    std::vector<Real>               chords_Histogram,  chords_Strength;
    std::vector<std::string>        chords_Progression;
    std::vector<std::vector<Real> > hpcp, hpcp_HighRes;
    
    void computeFeatures(const std::string song);
    
}


#endif  // FEATURES_H_INCLUDED
