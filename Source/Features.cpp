/*
  ==============================================================================

    Features.cpp
    Created: 24 Jul 2014 2:57:58pm
    Author:  milrob

  ==============================================================================
*/

#include "Features.h"
#include "JuceHeader.h"

using namespace fext;

    void computeFeatures(const std::string song){
        essentia::init();
        juce::ScopedPointer<Algorithm> audiofile       = AlgorithmFactory::create("MonoLoader", "filename", song,
                                                                                  "sampleRate", fext::SR);
        juce::ScopedPointer<Algorithm> fc              = AlgorithmFactory::create("FrameGenerator", "frameSize", FRAME_SIZE, "hopSize", HOP);
        juce::ScopedPointer<Algorithm> w               = AlgorithmFactory::create("Windowing", "type", wintype);
        juce::ScopedPointer<Algorithm> _spec           = AlgorithmFactory::create("Spectrum");
        juce::ScopedPointer<Algorithm> _mfcc           = AlgorithmFactory::create("MFCC");
        juce::ScopedPointer<Algorithm> _cent           = AlgorithmFactory::create("Centroid");
        juce::ScopedPointer<Algorithm> _onsetRate      = AlgorithmFactory::create("OnsetRate");
        juce::ScopedPointer<Algorithm> _rhythmExt      = AlgorithmFactory::create("RhythmExtractor2013");
        juce::ScopedPointer<Algorithm> _rms            = AlgorithmFactory::create("RMS");
        juce::ScopedPointer<Algorithm> _dynam          = AlgorithmFactory::create("DynamicComplexity");
        juce::ScopedPointer<Algorithm> _tonalExtractor = AlgorithmFactory::create("TonalExtractor");
 
        
        audiofile->output("audio").set(audioBuffer);
        fc->input("signal").set(audioBuffer);
        fc->output("frame").set(frame);
        w->input("signal").set(frame);
        w->output("windowedSignal").set(windowedFrame);
        _spec->input("signal").set(windowedFrame);
        _spec->output("spectrum").set(spectrum);

    
        
        /* ========= RHYTHM FEATURES ===================== */
        _onsetRate->input("signal")       .set(audioBuffer);
        _onsetRate->output("onsets")      .set(onsetTimes);
        _onsetRate->output("onsetRate")   .set(onsetRate);
        
        _rhythmExt->input("signal")       .set(audioBuffer);
        _rhythmExt->output("bpm")         .set(bpm);
        _rhythmExt->output("ticks")       .set(beats);
        _rhythmExt->output("confidence")  .set(beatsConfidence);
        _rhythmExt->output("bpmIntervals").set(beatIntervals);
        _rhythmExt->output("estimates")   .set(tempogram);
        
        /* ========= DYNAMICS FEATURES ===================== */
        _rms      ->input("signal")       .set(audioBuffer);
        _rms      ->output("RMS")         .set(rms);
        _dynam    ->input("signal")       .set(audioBuffer);
        _dynam    ->output("loudness")    .set(loudness);
        _dynam    ->output("dynamicComplexity").set(dynamicRangeCoeff);

        
        /* ========= TIMBRAL FEATURES ===================== */

        
        _mfcc->input("spectrum").set(spectrum);
        _mfcc->output("bands").set(mfccBands);
        _mfcc->output("coeffs").set(mfccCoeffs);
        _cent->input("spectrum").set(spectrum);
        _cent->output("centroid").set(centroid);
        
        /* ========= TONAL FEATURES ===================== */
        _tonalExtractor->output("chords_changes_rate").set(chords_ChangeRate);
        _tonalExtractor->output("chords_histogram")   .set(chords_Histogram);
        _tonalExtractor->output("chords_key")         .set(chords_key);
        _tonalExtractor->output("chords_number_rate") .set(chords_NumberRate);
        _tonalExtractor->output("chords_progression") .set(chords_Progression);
        _tonalExtractor->output("chords_scale")       .set(chords_Scale);
        _tonalExtractor->output("chords_strength")    .set(chords_Strength);
        _tonalExtractor->output("hpcp")               .set(hpcp);
        _tonalExtractor->output("hpcp_highres")       .set(hpcp_HighRes);
        _tonalExtractor->output("key_key")            .set(key_Key);
        _tonalExtractor->output("key_scale")          .set(key_Scale);
        
        
        
        /* ========= COMPUTE AND STORE FEATURES ===================== */
        audiofile->compute();
        _onsetRate->compute();
        _rhythmExt->compute();
        _tonalExtractor->compute();
        
        featureBin.add("rhythm.bpm", bpm);
        featureBin.add("rhythm.beats", beats);
        featureBin.add("rhythm.conf", beatsConfidence);
        featureBin.add("rhythm.intervals", beatIntervals);
        featureBin.add("rhythm.tempogram", tempogram);
        featureBin.add("rhythm.onsets", onsetTimes);
        featureBin.add("rhythm.rate", onsetRate);
        
        featureBin.add("tonal.chordrate", chords_ChangeRate);
        featureBin.add("tonal.histo", chords_Histogram);
        featureBin.add("tonal.chordsKey", chords_key);
        featureBin.add("tonal.chordsNumberRate", chords_NumberRate);
        featureBin.add("tonal.progression", chords_Progression);
        featureBin.add("tonal.chordScale", chords_Scale);
        featureBin.add("tonal.chordsStrength", chords_Strength);
       // featureBin.add("tonal.hpcp", hpcp);
       // featureBin.add("tonal.highres", hpcp_HighRes);
        featureBin.add("tonal.key", key_Key);
        featureBin.add("tonal.scale", key_Scale);
        
        while (true) {
            fc->compute();
            if (!frame.size()) break;
            if (isSilent(frame)) continue;
            w->compute();
            
            _rms->compute();
            _spec->compute();
            _mfcc->compute();
            _cent->compute();
            _dynam->compute();
            
            featureBin.add("dynam.loud", loudness);
            featureBin.add("dynam.rms", rms);
            featureBin.add("timbre.mfcc", mfccCoeffs);
            featureBin.add("timbre.cent", centroid);
        }
        
        
        essentia::shutdown();

    }







