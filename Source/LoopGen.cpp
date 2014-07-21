/*
  ==============================================================================

    LoopGen.cpp
    Created: 16 Jul 2014 2:23:14pm
    Author:  milrob

  ==============================================================================
*/

#include <iostream>
#include "JuceHeader.h"
#include "LoopGen.h"
#include "FeatureExtractor.h"

using namespace std;

float LAST_ONSET;
 vector<Loop> crudeLoops;

vector<Loop> computeLoops(const std::string audiofilename) {
    
    std::pair<vector<float>, float> onset_times_rate(computeOnsets(audiofilename));
    vector<float> onsets(onset_times_rate.first);
    LAST_ONSET = onsets.back();
    createLoops(onsets);
    vector<Loop> loops(crudeLoops);
    juce::String loopList;
    int itr = 0;
    for (const auto& lp : crudeLoops){
        loopList << "Loop " << itr << " is from " << lp.start << " to " << lp.end << "\n";
        //std::cout << "Loop " << itr << " is from " << lp.start << " to " << lp.end << std::endl;
        itr++;
    }
    juce::Logger* log = juce::Logger::getCurrentLogger();
    log->writeToLog(loopList);
    return loops;
}

inline void createLoops(const vector<float>& onsets){
    float lPoint;
    Loop curLoop;
    for (int i = 0; i < onsets.size(); ++i) {
        
        if (onsets[i] + BAR_SIZE <= LAST_ONSET) {
            //loop.start = static_cast<int>(onsets[i] * SR);
            curLoop.start = onsets[i];
            lPoint = onsets[i] + BAR_SIZE;
            //loop.end = static_cast<int>(quantizeToOnset(onsets, lPoint) * SR);
            curLoop.end = quantizeToOnset(onsets, lPoint);
            crudeLoops.push_back(curLoop);
        }
    }

}

inline void connectLoops(){
    for (int i = 0; i < crudeLoops.size(); ++i) {
        if (i == 0) {
            crudeLoops[i].prev = &crudeLoops[crudeLoops.size() - 1];
            crudeLoops[i].next = &crudeLoops[i+1];
        } else if (i == crudeLoops.size() - 1){
            crudeLoops[i].prev = &crudeLoops[i-1];
            crudeLoops[i].next = &crudeLoops[0];
        } else {
            crudeLoops[i].prev = &crudeLoops[i-1];
            crudeLoops[i].next = &crudeLoops[i+1];
        }
        if (crudeLoops[i].end > crudeLoops[i].start) {
            std::swap(crudeLoops[i].end, crudeLoops[i].start);
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
    
    if (diff1 > diff2)
        return onsets[limit.second - onsets.begin()];
    else
        return onsets[limit.first - onsets.begin()];
}

