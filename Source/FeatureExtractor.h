/*
  ==============================================================================

    onsetDetector.h
    Created: 15 Jul 2014 11:56:00am
    Author:  milrob

  ==============================================================================
*/

#ifndef ONSETDETECTOR_H_INCLUDED
#define ONSETDETECTOR_H_INCLUDED

#include "essentia.h"
#include "essentiamath.h"
#include "pool.h"
#include "algorithmfactory.h"

using namespace std;
using namespace essentia;
using namespace essentia::standard;

std::pair<vector<Real>, Real> computeOnsets(std::string sig){
    essentia::init();
    std::string audioFilename;
    vector<Real> audioBuffer;
    Algorithm* loader;
    
    audioFilename = sig;
    loader = AlgorithmFactory::create("MonoLoader", "filename", audioFilename, "sampleRate", 44100);
    loader->output("audio").set(audioBuffer);
    loader->compute();
    
    Real onsetRate;
    std::vector<Real> onsets;
    Algorithm* _onsetRate = AlgorithmFactory::create("OnsetRate");
    
    _onsetRate->input("signal").set(audioBuffer);
    _onsetRate->output("onsets").set(onsets);
    _onsetRate->output("onsetRate").set(onsetRate);
    
    _onsetRate->compute();
    
    std::pair<std::vector<Real>, Real> onsets_and_rate(onsets, onsetRate);
    
    delete _onsetRate;
    delete loader;
    essentia::shutdown();
    
    return onsets_and_rate;
}

#endif  // ONSETDETECTOR_H_INCLUDED
