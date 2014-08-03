/*
  ==============================================================================

    LoopGen.cpp
    Created: 16 Jul 2014 2:23:14pm
    Author:  milrob

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
    //const vector<essentia::Real> onsets(featureBin->value<vector<essentia::Real> >("rhythm.onsets"));
    const std::vector<essentia::Real> onsets(computeGlobalBeatsOnsets(audiofilename));
    LAST_ONSET = onsets.back();
    
    essentia::init();
    juce::ScopedPointer<essentia::standard::Algorithm> loader = essentia::standard::AlgorithmFactory::create("MonoLoader", "filename", audiofilename, "sampleRate", 44100);
    std::vector<essentia::Real> tempBuffer;
    loader->output("audio").set(tempBuffer);
    loader->compute();
    essentia::shutdown();
    
    createLoopPoints(onsets, tempBuffer);
    juce::String loopList;
    int itr = 0;
    for (auto& lp : theLoops){
        computeFeaturesForLoop(lp);
        loopList << "Loop " << itr << " is from " << lp.start << " to " << lp.end << "\n";
        itr++;
    }
    juce::Logger* log = juce::Logger::getCurrentLogger();
    log->writeToLog(loopList);
    delete log;
    
    return theLoops;
}

inline void createLoopPoints(const vector<float>& onsets, const std::vector<essentia::Real>& AUDIO_BUFFER){
    float lPoint;
    Loop curLoop;
    for (int i = 0; i < onsets.size(); ++i) {
        
        if (onsets[i] + BAR_SIZE <= LAST_ONSET) {
            //loop.start = static_cast<int>(onsets[i] * SR);
            curLoop.start = onsets[i];
            lPoint = onsets[i] + BAR_SIZE;
            //loop.end = static_cast<int>(quantizeToOnset(onsets, lPoint) * SR);
            curLoop.end = quantizeToOnset(onsets, lPoint);
            if (curLoop.start > curLoop.end) std::swap(curLoop.start, curLoop.end);
            auto first = AUDIO_BUFFER.begin() + curLoop.start;
            auto last = AUDIO_BUFFER.begin() + curLoop.end;
            std::vector<essentia::Real> tmp(first, last);
            curLoop.loopBuffer = tmp;
            theLoops.push_back(curLoop);
        }
    }

}
/*
   |Loop0|=> <=|Loop1|=> <=|Loop2|=> <=|LoopN|
 */
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



