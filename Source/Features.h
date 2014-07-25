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

const std::string SNDFILE;

namespace fext {
    
    //Utilities
    const int SR         = 44100;
    const int FRAME_SIZE = 2048;
    const int HOP        = FRAME_SIZE / 2;
    std::string wintype  = "hann";
    Pool CURATED_FEATURES;
    std::vector<Real> audioBuffer, wFrame, spectrum;
    
    
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
    
    //Methods
    void initSys(const std::string& song);
    void ExtractAndStoreFeatures(const std::string& song);
    void computeRhythmFeatures();
    void computeDynamicsFeatures();
    void computeTimbralFeatures();
    void computePitchFeatures(const std::string& song);
    
    //Use these two methods instead of the above
    void computeTemporalFeatures();
    void computeSpectralFeatures();
    
}


#endif  // FEATURES_H_INCLUDED