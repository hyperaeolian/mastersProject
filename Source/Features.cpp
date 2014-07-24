/*
  ==============================================================================

    Features.cpp
    Created: 24 Jul 2014 2:57:58pm
    Author:  milrob

  ==============================================================================
*/

#include "Features.h"
#include "JuceHeader.h"

namespace fext{

    void initSys(const std::string &song){
        essentia::init();
        juce::ScopedPointer<Algorithm> audiofile = AlgorithmFactory::create("MonoLoader", "filename", song,
                                                        "sampleRate", fext::SR);
        juce::ScopedPointer<Algorithm> fc = AlgorithmFactory::create("FrameGenerator", "frameSize", FRAME_SIZE, "hopSize", HOP);
        juce::ScopedPointer<Algorithm> w = AlgorithmFactory::create("Windowing", "type", wintype);
        juce::ScopedPointer<Algorithm> spec = AlgorithmFactory::create("Spectrum");
        
        std::vector<Real> frame;
        audiofile->output("audio").set(audioBuffer);
        fc->input("signal").set(audioBuffer);
        fc->output("frame").set(frame);
        w->input("signal").set(frame);
        w->output("windowedSignal").set(wFrame);
        spec->input("signal").set(wFrame);
        spec->output("spectrum").set(spectrum);
        
        audiofile->compute();
        while (true) {
            fc->compute();
            if (!frame.size()) break;
            if (isSilent(frame)) continue;
            w->compute();
        }

    }

    void computeRhythmFeatures(){
        /*
            BPM, onsetRate, onsetTimes, beats, beatsConfidence, beatIntervals, tempogram
         */
        juce::ScopedPointer<Algorithm> _onsetRate = AlgorithmFactory::create("OnsetRate");
        juce::ScopedPointer<Algorithm> _rhythmExt = AlgorithmFactory::create("RhythmExtractor2013");
        
        _onsetRate->input("signal")       .set(audioBuffer);
        _onsetRate->output("onsets")      .set(onsetTimes);
        _onsetRate->output("onsetRate")   .set(onsetRate);
        
        _rhythmExt->input("signal")       .set(audioBuffer);
        _rhythmExt->output("bpm")         .set(bpm);
        _rhythmExt->output("ticks")       .set(beats);
        _rhythmExt->output("confidence")  .set(beatsConfidence);
        _rhythmExt->output("bpmIntervals").set(beatIntervals);
        _rhythmExt->output("estimates")   .set(tempogram);
        
        _onsetRate->compute();      _rhythmExt->compute();

    }

    void computeDynamicsFeatures(){
       //TODO: Add framed versions of these features
       //Avoid recomputing a framed signal, perhaps a for_each macro
        
        /*
            dynamicRangeCoeffs, RMS, Loudness,
         */
        
        juce::ScopedPointer<Algorithm> _rms = AlgorithmFactory::create("RMS");
        juce::ScopedPointer<Algorithm> _dynam = AlgorithmFactory::create("DynamicComplexity");
        
        _rms->input("signal")              .set(audioBuffer);
        _rms->output("RMS")                .set(rms);
        _dynam->input("signal")            .set(audioBuffer);
        _dynam->output("dynamicComplexity").set(dynamicRangeCoeff);
        _dynam->output("loudness")         .set(loudness);
        
        _rms->compute();     _dynam->compute();
    }
    
    void computeTimbralFeatures(){
        /*
            mfccBands, mfccCoeffs, spectralCentroid
         */
        
        juce::ScopedPointer<Algorithm> _mfcc = AlgorithmFactory::create("MFCC");
        juce::ScopedPointer<Algorithm> _cent = AlgorithmFactory::create("Centroid");
        
        _mfcc->input("spectrum").set(spectrum);
        _mfcc->output("bands").set(mfccBands);
        _mfcc->output("coeffs").set(mfccCoeffs);
        
    }


}







