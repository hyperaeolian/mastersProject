/*
  ==============================================================================

    Here, the audio buffer is initialized; global onsets and beats are detected;
        loop points are created using those onsets/beats; features for each
        created loop is then extracted

  ==============================================================================
*/

#include "LoopGenerator.h"
#include "FeatureExtractor.h"

/*
using namespace std;

const int SR = 44100;
const float BAR_SIZE = 1.0;

float LAST_ONSET;


vector<Loop> computeLoops(const std::string audiofilename, essentia::Real& bpm) {
    std::vector<Loop> theLoops;
    const std::vector<essentia::Real> onsets(computeGlobalBeatsOnsets(audiofilename, bpm)); //actually returning beats not onsets
    LAST_ONSET = onsets.back();
    
    essentia::init();
        juce::ScopedPointer<essentia::standard::Algorithm> loader = essentia::standard::AlgorithmFactory::create("MonoLoader",
                                                                                                                 "filename", audiofilename, "sampleRate", SR);
        std::vector<essentia::Real> tempBuffer;
        loader->output("audio").set(tempBuffer);
        loader->compute();
    essentia::shutdown();
    
    BackgroundThread progressWindow(static_cast<int>(onsets.size()), "Loop Generator");
    if (progressWindow.runThread()){
        progressWindow.setStatusMessage("Finding all possible loop points...");
        createLoopPoints(onsets, tempBuffer, theLoops);
        connectLoops(theLoops);
        //Compute features for each loop
        progressWindow.setStatusMessage("Computing features for loops...");
        for (auto& lp : theLoops){
            std::vector<_REAL> loopBuffer(tempBuffer.begin() + lp.head, tempBuffer.begin() + lp.tail);
            computeFeaturesForLoop(lp, loopBuffer);
            
            //Find # of beats in each loop
            for (const auto& b : onsets){
                if (b*SR >= lp.head && b*SR <= lp.tail) lp.numBeats++;
            }
        }
    } else
        progressWindow.threadComplete(true);
   
    return theLoops;
}

void createLoopPoints(const std::vector<float>& onsets, const std::vector<_REAL>& AUDIO_BUFFER, std::vector<Loop>& loops){
    float lPoint;
    Loop curr;

    for (int i = 0; i < onsets.size(); ++i) {
        if (onsets[i] + BAR_SIZE <= LAST_ONSET) {
            curr.start = onsets[i];
            lPoint     = onsets[i] + BAR_SIZE;
            curr.end   = quantizeToOnset(onsets, lPoint);
            if (curr.start < curr.end)
                std::swap(curr.start, curr.end);
            curr.head = static_cast<int>(onsets[i] * SR);
            curr.tail = static_cast<int>(onsets[i+1] * SR);
            loops.push_back(curr);
        }
    }

}

void connectLoops(std::vector<Loop>& loops){
    for (int i = 0; i < loops.size(); ++i) {
        if (i == 0) {
            loops[i].prev = &loops[i];
            loops[i].next = &loops[1];
        } else if (i == loops.size() - 1){
            loops[i].prev = &loops[i-1];
            loops[i].next = &loops[i];
        } else {
            loops[i].prev = &loops[i-1];
            loops[i].next = &loops[i+1];
        }
        if (loops[i].end < loops[i].start) {
            std::swap(loops[i].end, loops[i].start);
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

*/

namespace lgen{

LoopGenerator::LoopGenerator(const std::vector<_REAL>& _AudioBuffer, const std::vector<_REAL>& _delim) :
    BarSize(1.0), AudioBuffer(_AudioBuffer), delimiters(_delim)
{
    lastDelimiter = delimiters.back();
    
}

LoopGenerator::~LoopGenerator(){}

void LoopGenerator::createLoopPoints(){
    
    Loop curr;
    for (int i = 0; i < delimiters.size(); ++i) {
        if (delimiters[i] + BarSize <= lastDelimiter) {
            curr.start  = delimiters[i];
            _REAL point = delimiters[i] + BarSize;
            curr.end    = quantizeToDelimiter(point);
            if (curr.start < curr.end)
                std::swap(curr.start, curr.end);
            curr.head = static_cast<int>(curr.start * SR);
            curr.tail = static_cast<int>(curr.end * SR);
            //curr.head = static_cast<int>(delimiters[i] * SR);
            //curr.tail = static_cast<int>(delimiters[i+1] * SR);
            _Loops.push_back(curr);
        }
    }
}

void LoopGenerator::connectLoops(){
    for (int i = 0; i < _Loops.size(); ++i) {
        if (i == 0) {
            _Loops[i].prev = &_Loops[i];
            _Loops[i].next = &_Loops[1];
        } else if (i == _Loops.size() - 1){
            _Loops[i].prev = &_Loops[i-1];
            _Loops[i].next = &_Loops[i];
        } else {
            _Loops[i].prev = &_Loops[i-1];
            _Loops[i].next = &_Loops[i+1];
        }
    }
}

_REAL LoopGenerator::quantizeToDelimiter(_REAL value){
    if (value > lastDelimiter) {
        value = lastDelimiter;
        return value;
    }
    auto limit = std::equal_range(delimiters.begin(), delimiters.end(), value);
    float diff1 = abs(value - delimiters[limit.first - delimiters.begin()]);
    float diff2 = abs(delimiters[limit.second - delimiters.begin()] - value);
    
    if (diff1 > diff2) return delimiters[limit.second - delimiters.begin()];
    else               return delimiters[limit.first - delimiters.begin()];
}
    
    
//Helper functions
    
    bool audioBuffered;
    
//==========================Buffer Audio========================================
    void initAudio(const std::string audiofilename){
        essentia::init();
        juce::ScopedPointer<essentia::standard::Algorithm>
            loader = essentia::standard::AlgorithmFactory::create("MonoLoader",
                                                                  "filename", audiofilename,
                                                                  "sampleRate", 44100);
        loader->output("audio").set(lgen::AudioBuffer);
        loader->compute();
        lgen::audioBuffered = true;
        essentia::shutdown();
       
    }
    
//==========================Compute Loops=======================================
    std::vector<Loop> computeLoops(){
        
        if (lgen::audioBuffered){
            std::unique_ptr<FeatureExtractor> xtractor(new FeatureExtractor(lgen::AudioBuffer));
            
            //Can use either onsets or beats as loop points
            //xtractor->findOnsets();
            xtractor->findBeats();
            
            std::unique_ptr<LoopGenerator> loopGen(new LoopGenerator(lgen::AudioBuffer,
                                                                     xtractor->getBeats()));
        
            BackgroundThread progressWindow(static_cast<int>(loopGen->getNumLoopsToCreate()), "Loop Generator");
            if (progressWindow.runThread()){
                progressWindow.setStatusMessage("Finding all possible loop points...");
                loopGen->createLoopPoints();
                loopGen->connectLoops();
         
                //Compute features for each loop
                progressWindow.setStatusMessage("Computing features for loops...");
               // std::vector<_REAL> buffer(loopGen->accessBuffer());
                for (auto& lp : loopGen->getLoops()) xtractor->computeFeaturesForLoop(lp);
            } else
                progressWindow.threadComplete(true);
            
            return loopGen->getLoops();
        }
        
    }
    
    
} //end namespace