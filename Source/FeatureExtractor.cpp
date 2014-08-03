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

const int FRAME_SIZE = 2048;
const int HOP = FRAME_SIZE / 2;
const int NumFeatures = 25;
bool successfulExtraction = false;

essentia::Pool* featureBin = new Pool;

void computeFeatures(std::string song){
    BackgroundThread progress(NumFeatures);
    if (progress.runThread()){
        essentia::init();
        AlgorithmFactory& factory = essentia::standard::AlgorithmFactory::instance();
        juce::ScopedPointer<Algorithm>
            audiofile       = factory.create("MonoLoader", "filename", song, "sampleRate", 44100),
            _dur            = factory.create("Duration"),
            fc              = factory.create("FrameCutter", "frameSize", FRAME_SIZE, "hopSize", HOP),
            w               = factory.create("Windowing", "type", "hann"),
            _spec           = factory.create("Spectrum"),
            _mfcc           = factory.create("MFCC"),
            _cent           = factory.create("Centroid"),
            _onsetRate      = factory.create("OnsetRate"),
            _rhythmExt      = factory.create("RhythmExtractor2013"),
            _rms            = factory.create("RMS"),
            _dynam          = factory.create("DynamicComplexity"),
            _tonalExtractor = factory.create("TonalExtractor");
        
        
        std::vector<Real> audioBuffer, frame, windowedFrame, spectrum;
        Real duration;
        
        audiofile->output("audio").set(audioBuffer);
        _dur     ->input("signal").set(audioBuffer);
        _dur     ->output("duration").set(duration);
        fc       ->input("signal").set(audioBuffer);
        fc       ->output("frame").set(frame);
        w        ->input("frame").set(frame);
        w        ->output("frame").set(windowedFrame);
        _spec    ->input("frame").set(windowedFrame);
        _spec    ->output("spectrum").set(spectrum);
        
        
        /* ========= RHYTHM FEATURES ===================== */
        Real bpm, onsetRate, beatsConfidence;
        std::vector<Real> tempogram, onsetTimes, beats, beatIntervals;
        
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
        Real dynamicRangeCoeff, loudness, rms;
        //std::vector<Real> rms;
        
        _rms      ->input("array")       .set(frame);
        _rms      ->output("rms")         .set(rms);
        _dynam    ->input("signal")       .set(audioBuffer);
        _dynam    ->output("loudness")    .set(loudness);
        _dynam    ->output("dynamicComplexity").set(dynamicRangeCoeff);
        
        /* ========= TIMBRAL FEATURES ===================== */
        std::vector<Real> mfccBands, mfccs;
        Real centroid;
        
        _mfcc->input("spectrum").set(spectrum);
        _mfcc->output("bands").set(mfccBands);
        _mfcc->output("mfcc").set(mfccs);
        _cent->input("array").set(spectrum);
        _cent->output("centroid").set(centroid);
        
        /* ========= TONAL FEATURES ===================== */
        Real                            chords_ChangeRate, chords_NumberRate;
        Real                            key_Strength;
        std::string                     chords_key, chords_Scale;
        std::string                     key_Key, key_Scale;
        std::vector<Real>               chords_Histogram,  chords_Strength;
        std::vector<std::string>        chords_Progression;
        std::vector<std::vector<Real> > hpcp, hpcp_HighRes;
        
        _tonalExtractor->input("signal").set(audioBuffer);
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
        _tonalExtractor->output("key_strength")          .set(key_Strength);
        
        
        
        /* ========= COMPUTE AND STORE FEATURES ===================== */
        
        audiofile->compute();
        _dur->compute();
        _onsetRate->compute();
        _rhythmExt->compute();
        _tonalExtractor->compute();
        
        
        featureBin->set("rhythm.bpm", bpm);
        featureBin->set("rhythm.beats", beats);
        featureBin->set("rhythm.conf", beatsConfidence);
        featureBin->set("rhythm.intervals", beatIntervals);
        featureBin->set("rhythm.tempogram", tempogram);
        featureBin->set("rhythm.onsets", onsetTimes);
        featureBin->set("rhythm.rate", onsetRate);
        
        featureBin->set("tonal.chordrate", chords_ChangeRate);
        featureBin->set("tonal.histo", chords_Histogram);
        featureBin->set("tonal.chordsKey", chords_key);
        featureBin->set("tonal.chordsNumberRate", chords_NumberRate);
        featureBin->add("tonal.progression", chords_Progression);
        featureBin->set("tonal.chordScale", chords_Scale);
        featureBin->set("tonal.chordsStrength", chords_Strength);
        // featureBin.add("tonal.hpcp", hpcp);
        // featureBin.add("tonal.highres", hpcp_HighRes);
        featureBin->set("tonal.key", key_Key);
        featureBin->set("tonal.scale", key_Scale);
        
        
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
            
            featureBin->set("dynam.loud", loudness);
            featureBin->set("dynam.rms", rms);
            featureBin->set("timbre.mfcc", mfccs);
            featureBin->set("timbre.cent", centroid);
        }
        
        essentia::shutdown();
    } else {
      //  progress.threadComplete(true);
        successfulExtraction = true;
    }
    
    
}


std::vector<essentia::Real> computeGlobalBeatsOnsets(std::string song){
    essentia::init();
    juce::ScopedPointer<Algorithm> audiofile = AlgorithmFactory::create("MonoLoader", "filename", song,
                                                              "sampleRate", 44100);
    juce::ScopedPointer<Algorithm> _onsetRate = AlgorithmFactory::create("OnsetRate"),
                                   _beatTrack = AlgorithmFactory::create("BeatTrackerMultiFeature");
    std::vector<Real> buffer, onsetTimes, beats;
    Real rate, conf;
    audiofile->output("audio").set(buffer);
    _onsetRate->input("signal").set(buffer);
    _onsetRate->output("onsets").set(onsetTimes);
    _onsetRate->output("onsetRate").set(rate);
    _beatTrack->input("signal").set(buffer);
    _beatTrack->output("ticks").set(beats);
    _beatTrack->output("confidence").set(conf);
    
    audiofile->compute();
    _onsetRate->compute();
    _beatTrack->compute();
    
    essentia::shutdown();
    return onsetTimes;
    
    //TODO: Get the beats later
    
}

void computeFeaturesForLoop(Loop& loop){
        essentia::init();
        AlgorithmFactory& factory = essentia::standard::AlgorithmFactory::instance();
        juce::ScopedPointer<Algorithm>
        _dur            = factory.create("Duration"),
        fc              = factory.create("FrameCutter", "frameSize", FRAME_SIZE, "hopSize", HOP),
        w               = factory.create("Windowing", "type", "hann"),
        _spec           = factory.create("Spectrum"),
        _mfcc           = factory.create("MFCC"),
        _cent           = factory.create("Centroid"),
        _onsetRate      = factory.create("OnsetRate"),
        _rhythmExt      = factory.create("RhythmExtractor2013"),
        _rms            = factory.create("RMS"),
        _dynam          = factory.create("DynamicComplexity"),
        _tonalExtractor = factory.create("TonalExtractor");
        
        
        std::vector<Real> frame, windowedFrame, spectrum;
        Real duration;
        
        _dur     ->input("signal").set(loop.loopBuffer);
        _dur     ->output("duration").set(duration);
        fc       ->input("signal").set(loop.loopBuffer);
        fc       ->output("frame").set(frame);
        w        ->input("frame").set(frame);
        w        ->output("frame").set(windowedFrame);
        _spec    ->input("frame").set(windowedFrame);
        _spec    ->output("spectrum").set(spectrum);
        
        
        /* ========= RHYTHM FEATURES ===================== */
        Real bpm, onsetRate, beatsConfidence;
        std::vector<Real> tempogram, onsetTimes, beats, beatIntervals;
        
        _onsetRate->input("signal")       .set(loop.loopBuffer);
        _onsetRate->output("onsets")      .set(onsetTimes);
        _onsetRate->output("onsetRate")   .set(onsetRate);
        
        _rhythmExt->input("signal")       .set(loop.loopBuffer);
        _rhythmExt->output("bpm")         .set(bpm);
        _rhythmExt->output("ticks")       .set(beats);
        _rhythmExt->output("confidence")  .set(beatsConfidence);
        _rhythmExt->output("bpmIntervals").set(beatIntervals);
        _rhythmExt->output("estimates")   .set(tempogram);
        
        /* ========= DYNAMICS FEATURES ===================== */
        Real dynamicRangeCoeff, loudness, rms;
        //std::vector<Real> rms;
        
        _rms      ->input("array")       .set(frame);
        _rms      ->output("rms")         .set(rms);
        _dynam    ->input("signal")       .set(loop.loopBuffer);
        _dynam    ->output("loudness")    .set(loudness);
        _dynam    ->output("dynamicComplexity").set(dynamicRangeCoeff);
        
        /* ========= TIMBRAL FEATURES ===================== */
        std::vector<Real> mfccBands, mfccs;
        Real centroid;
        
        _mfcc->input("spectrum").set(spectrum);
        _mfcc->output("bands").set(mfccBands);
        _mfcc->output("mfcc").set(mfccs);
        _cent->input("array").set(spectrum);
        _cent->output("centroid").set(centroid);
        
        /* ========= TONAL FEATURES ===================== */
        Real                            chords_ChangeRate, chords_NumberRate;
        Real                            key_Strength;
        std::string                     chords_key, chords_Scale;
        std::string                     key_Key, key_Scale;
        std::vector<Real>               chords_Histogram,  chords_Strength;
        std::vector<std::string>        chords_Progression;
        std::vector<std::vector<Real> > hpcp, hpcp_HighRes;
        
        _tonalExtractor->input("signal").set(loop.loopBuffer);
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
        _tonalExtractor->output("key_strength")          .set(key_Strength);
        
        
        
        /* ========= COMPUTE AND STORE FEATURES ===================== */
        
        _dur->compute();
        _onsetRate->compute();
        _rhythmExt->compute();
        _tonalExtractor->compute();
        
        
        loop.bin.set("rhythm.bpm", bpm);
        loop.bin.set("rhythm.beats", beats);
        loop.bin.set("rhythm.conf", beatsConfidence);
        loop.bin.set("rhythm.intervals", beatIntervals);
        loop.bin.set("rhythm.tempogram", tempogram);
        loop.bin.set("rhythm.onsets", onsetTimes);
        loop.bin.set("rhythm.rate", onsetRate);
        
        loop.bin.set("tonal.chordrate", chords_ChangeRate);
        loop.bin.set("tonal.histo", chords_Histogram);
        loop.bin.set("tonal.chordsKey", chords_key);
        loop.bin.set("tonal.chordsNumberRate", chords_NumberRate);
       // loop.bin.set("tonal.progression", chords_Progression);
        loop.bin.set("tonal.chordScale", chords_Scale);
        loop.bin.set("tonal.chordsStrength", chords_Strength);
        //loop.bin.set("tonal.hpcp", hpcp);
        //loop.bin.set("tonal.highres", hpcp_HighRes);
        loop.bin.set("tonal.key", key_Key);
        loop.bin.set("tonal.scale", key_Scale);
        
        
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
            
            loop.bin.set("dynam.loud", loudness);
            loop.bin.set("dynam.rms", rms);
            loop.bin.set("timbre.mfcc", mfccs);
            loop.bin.set("timbre.cent", centroid);
        }
        
        
        
        essentia::shutdown();
  
}





