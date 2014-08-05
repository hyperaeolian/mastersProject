/*
  ==============================================================================

    Here, the audio buffer is initialized; global onsets and beats are detected;
        loop points are created using those onsets/beats; features for each
        created loop is then extracted

  ==============================================================================
*/

#include "LoopGenerator.h"
#include "FeatureExtractor.h"


using namespace std;

const int SR = 44100;
const float BAR_SIZE = 1.0;


std::vector<Loop> theLoops;
float LAST_ONSET;


vector<Loop> computeLoops(std::string audiofilename) {
    const std::vector<essentia::Real> onsets(computeGlobalBeatsOnsets(audiofilename));
    LAST_ONSET = onsets.back();
    
    essentia::init();
        juce::ScopedPointer<essentia::standard::Algorithm> loader = essentia::standard::AlgorithmFactory::create("MonoLoader", "filename", audiofilename, "sampleRate", 44100);
        std::vector<essentia::Real> tempBuffer;
        loader->output("audio").set(tempBuffer);
        loader->compute();
    essentia::shutdown();
    juce::String s("Initializing Loop Generator");
    BackgroundThread progressWindow(static_cast<int>(onsets.size()), s);
    
    if (progressWindow.runThread()){
        progressWindow.setStatusMessage("Finding all possible loop points...");
        createLoopPoints(onsets, tempBuffer);
        progressWindow.setStatusMessage("Computing features for loops...");
        for (auto& lp : theLoops)
            computeFeaturesForLoop(lp);
    } else {
        progressWindow.threadComplete(true);
    }
    
    return theLoops;
}

void createLoopPoints(const vector<float>& onsets, const std::vector<essentia::Real>& AUDIO_BUFFER){
    float lPoint;
    Loop curr;
    for (int i = 0; i < onsets.size(); ++i) {
        if (onsets[i] + BAR_SIZE <= LAST_ONSET) {
            curr.start = onsets[i];
            lPoint     = onsets[i] + BAR_SIZE;
            curr.end   = quantizeToOnset(onsets, lPoint);
            if (curr.start > curr.end)
                std::swap(curr.start, curr.end);
            auto first = AUDIO_BUFFER.begin() + curr.start;
            auto last  = AUDIO_BUFFER.begin() + curr.end;
            std::vector<essentia::Real> tmp(first, last);
            curr.loopBuffer = tmp;
            theLoops.push_back(curr);
        }
    }

}

void connectLoops(){
    for (int i = 0; i < theLoops.size(); ++i) {
        if (i == 0) {
            theLoops[i].prev = &theLoops[i];
            theLoops[i].next = &theLoops[1];
        } else if (i == theLoops.size() - 1){
            theLoops[i].prev = &theLoops[i-1];
            theLoops[i].next = &theLoops[i];
        } else {
            theLoops[i].prev = &theLoops[i-1];
            theLoops[i].next = &theLoops[i+1];
        }
        if (theLoops[i].end > theLoops[i].start) {
            std::swap(theLoops[i].end, theLoops[i].start);
        }
    }
}


inline float quantizeToOnset(const vector<float>& onsets, float value){
    if (value > LAST_ONSET) {
        value = LAST_ONSET;
        return value;
    }
    auto limit = std::equal_range(onsets.begin(), onsets.end(), value);
    float diff1 = abs(value - onsets[limit.first - onsets.begin()]);
    float diff2 = abs(onsets[limit.second - onsets.begin()] - value);
    
    if (diff1 > diff2) return onsets[limit.second - onsets.begin()];
    else               return onsets[limit.first - onsets.begin()];
}



