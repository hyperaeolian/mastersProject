/*
  ==============================================================================

    Features.cpp
    Created: 24 Jul 2014 2:57:58pm
    Author:  milrob

  ==============================================================================
*/


#include "FeatureExtractor.h"

using namespace essentia;
using namespace essentia::standard;

FeatureExtractor::FeatureExtractor(const VEC_REAL& buffer) :
    AudioBuffer(buffer), SR(44100), FrameSize(2048), HopSize(FrameSize/2)

{
    essentia::init();
}

FeatureExtractor::~FeatureExtractor(){
    essentia::shutdown();
}

void FeatureExtractor::computeFeaturesForBuffer(){
    AlgorithmFactory& factory = essentia::standard::AlgorithmFactory::instance();
    juce::ScopedPointer<Algorithm> rhythmXtractor = factory.create("RhythmExtractor2013");
    juce::ScopedPointer<Algorithm> onsetDetector = factory.create("OnsetRate");
    
    VEC_REAL estimates, bpmIntervals;
    Real confidence, rate, bpm;
    
    rhythmXtractor->input("signal").set(AudioBuffer);
    rhythmXtractor->output("bpm").set(bpm);
    rhythmXtractor->output("ticks").set(beats);
    rhythmXtractor->output("confidence").set(confidence);
    rhythmXtractor->output("estimates").set(estimates);
    rhythmXtractor->output("bpmIntervals").set(bpmIntervals);
    
    onsetDetector->input("signal").set(AudioBuffer);
    onsetDetector->output("onsets").set(onsets);
    onsetDetector->output("onsetRate").set(rate);
    
    rhythmXtractor->compute();
    onsetDetector->compute();

}

void FeatureExtractor::computeFeaturesForLoop(Loop& loop){
    AlgorithmFactory& factory = essentia::standard::AlgorithmFactory::instance();

    auto first = AudioBuffer.begin() + loop.sampsStart;
    auto second = AudioBuffer.begin() + loop.sampsEnd;

    VEC_REAL loopBuffer(first, second);
   
    juce::ScopedPointer<Algorithm>
            _fc             = factory.create("FrameCutter", "frameSize", FrameSize,
                                             "hopSize", HopSize),
            _w              = factory.create("Windowing", "type", "hann"),
            _spec           = factory.create("Spectrum"),
            _cent           = factory.create("Centroid"),
            _rms            = factory.create("RMS"),
            _rhythmExt      = factory.create("RhythmExtractor2013"),
            _dynam          = factory.create("DynamicComplexity"),
            _tonalExtractor = factory.create("TonalExtractor");


        VEC_REAL frame, windowedFrame, spectrum;

        _fc      ->input("signal").set(loopBuffer);
        _fc      ->output("frame").set(frame);
        _w       ->input("frame").set(frame);
        _w       ->output("frame").set(windowedFrame);
        _spec    ->input("frame").set(windowedFrame);
        _spec    ->output("spectrum").set(spectrum);

        /* ========= RHYTHM FEATURES ===================== */
        Real bpm, beatsConfidence;
        VEC_REAL tempogram, beats, beatIntervals;

        _rhythmExt->input("signal")       .set(loopBuffer);
        _rhythmExt->output("bpm")         .set(bpm);
        _rhythmExt->output("ticks")       .set(beats);
        _rhythmExt->output("confidence")  .set(beatsConfidence);
        _rhythmExt->output("bpmIntervals").set(beatIntervals);
        _rhythmExt->output("estimates")   .set(tempogram);
    
        _rhythmExt->compute();
    
        loop.bin.set("rhythm.bpm", bpm);
        loop.bin.set("rhythm.beats", beats);
        loop.bin.set("rhythm.conf", beatsConfidence);
        loop.bin.set("rhythm.intervals", beatIntervals);
        loop.bin.set("rhythm.tempogram", tempogram);

        /* ========= DYNAMICS FEATURES ===================== */
        Real dynamicRangeCoeff, loudness, rms;

        _rms    ->  input("array")             .set(frame);
        _rms    ->  output("rms")              .set(rms);
        _dynam  ->  input("signal")            .set(loopBuffer);
        _dynam  ->  output("loudness")         .set(loudness);
        _dynam  ->  output("dynamicComplexity").set(dynamicRangeCoeff);
    
        //Spectrum//
        _fc->compute();
        _w->compute();
        _spec->compute();
    
        _dynam->compute();
        _rms->compute();
    
        loop.bin.set("dynam.rms", rms);
        loop.bin.set("dynam.loud", loudness);
        loop.bin.set("dynam.dyRange", dynamicRangeCoeff);
    
        /* ========= TIMBRAL FEATURES ===================== */
       // VEC_REAL mfccBands, mfccs;
        Real centroid;

       // _mfcc->input("spectrum").set(spectrum);
       // _mfcc->output("bands").set(mfccBands);
       // _mfcc->output("mfcc").set(mfccs);
        _cent->input("array").set(spectrum);
        _cent->output("centroid").set(centroid);
    
        _cent->compute();
    
        loop.bin.set("timbre.cent", centroid);

        /* ========= TONAL FEATURES ===================== */
        Real                            chords_ChangeRate, chords_NumberRate;
        Real                            key_Strength;
        std::string                     chords_key, chords_Scale;
        std::string                     key_Key, key_Scale;
        VEC_REAL                        chords_Histogram,  chords_Strength;
        std::vector<std::string>        chords_Progression;
        std::vector<VEC_REAL>           hpcp, hpcp_HighRes;

        _tonalExtractor->input("signal")              .set(loopBuffer);
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
        _tonalExtractor->output("key_strength")       .set(key_Strength);

        _tonalExtractor->compute();
    
        loop.bin.set("tonal.chordrate", chords_ChangeRate);
        loop.bin.set("tonal.histo", chords_Histogram);
        loop.bin.set("tonal.chordsKey", chords_key);
        loop.bin.set("tonal.chordsNumberRate", chords_NumberRate);
        loop.bin.set("tonal.chordScale", chords_Scale);
        loop.bin.set("tonal.chordsStrength", chords_Strength);
        loop.bin.set("tonal.key", key_Key);
        loop.bin.set("tonal.scale", key_Scale);
        loop.bin.set("tonal.keyStr", key_Strength);

}






