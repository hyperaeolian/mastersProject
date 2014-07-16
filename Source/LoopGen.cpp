/*
  ==============================================================================

    LoopGen.cpp
    Created: 16 Jul 2014 2:23:14pm
    Author:  milrob

  ==============================================================================
*/

#include <iostream>
#include "LoopGen.h"
#include "FeatureExtractor.h"


using namespace std;
float LAST_ONSET;
vector<Loop> LOOP_REPO;

vector<Loop> computeLoops(const std::string audiofilename) {
    
    std::pair<vector<float>, float> onset_times_rate(computeOnsets(audiofilename));
    vector<float> onsets(onset_times_rate.first);
    LAST_ONSET = onsets[onsets.size()-1];
    createLoops(onsets);
    vector<Loop> loops(LOOP_REPO);
    return loops;
}

inline void createLoops(const vector<float>& onsets){
    float lPoint;
    Loop loop;
    for (int i = 0; i < onsets.size(); ++i) {
        if (onsets[i] + BAR_SIZE > LAST_ONSET) {
            //loop.start = static_cast<int>(onsets[i] * SR);
            loop.start = onsets[i];
            lPoint = onsets[i] + BAR_SIZE;
            //loop.end = static_cast<int>(quantizeToOnset(onsets, lPoint) * SR);
            loop.end = quantizeToOnset(onsets, lPoint);
            LOOP_REPO.push_back(loop);
        }
    }
}

inline void connectLoops(){
    for (int i = 0; i < LOOP_REPO.size(); ++i) {
        if (i == 0) {
            LOOP_REPO[i].prev = &LOOP_REPO[LOOP_REPO.size() - 1];
            LOOP_REPO[i].next = &LOOP_REPO[i+1];
        } else if (i == LOOP_REPO.size() - 1){
            LOOP_REPO[i].prev = &LOOP_REPO[i-1];
            LOOP_REPO[i].next = &LOOP_REPO[0];
        } else {
            LOOP_REPO[i].prev = &LOOP_REPO[i-1];
            LOOP_REPO[i].next = &LOOP_REPO[i+1];
        }
        if (LOOP_REPO[i].end > LOOP_REPO[i].start) {
            std::swap(LOOP_REPO[i].end, LOOP_REPO[i].start);
        }
    }
}


inline float quantizeToOnset(const vector<float>& onsets, float value){
    if (value > LAST_ONSET) {
        value /= LAST_ONSET;
    }
    auto limit = std::equal_range(onsets.begin(), onsets.end(), value);
    float diff1 = abs(value - onsets[limit.first - onsets.begin()]);
    float diff2 = abs(onsets[limit.second - onsets.begin()] - value);
    
    if (diff1 > diff2)
        return onsets[limit.second - onsets.begin()];
    else
        return onsets[limit.first - onsets.begin()];
}

