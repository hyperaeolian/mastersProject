/*
  ==============================================================================

    Here, the audio buffer is initialized; global onsets and beats are detected;
        loop points are created using those onsets/beats; features for each
        created loop is then extracted

  ==============================================================================
*/

#include "LoopGenerator.h"
#include "FeatureExtractor.h"


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
    
    
//namespace (non-member) helper functions
    
    bool audioBuffered;
    
//==========================Buffer Audio========================================
    std::vector<_REAL> initAudio(const std::string audiofilename){
        essentia::init();
        juce::ScopedPointer<essentia::standard::Algorithm>
            loader = essentia::standard::AlgorithmFactory::create("MonoLoader",
                                                                  "filename", audiofilename,
                                                                  "sampleRate", 44100);
        std::vector<_REAL> buffer;
        loader->output("audio").set(buffer);
        loader->compute();
        lgen::audioBuffered = true;
        essentia::shutdown();
        
        return buffer;
       
    }
    
//==========================Compute Loops=======================================
    std::vector<Loop> computeLoops(const std::vector<_REAL>& buffer){
        
        if (lgen::audioBuffered){
            std::unique_ptr<FeatureExtractor> xtractor(new FeatureExtractor(buffer));
            
            //Can use either onsets or beats as loop points
            //xtractor->findOnsets();
            xtractor->findBeats();
            
            std::unique_ptr<LoopGenerator> loopGen(new LoopGenerator(buffer,
                                                                     xtractor->getBeats()));
        
            BackgroundThread progressWindow(static_cast<int>(loopGen->getNumLoopsToCreate()), "Loop Generator");
            if (progressWindow.runThread()){
                progressWindow.setStatusMessage("Finding all possible loop points...");
                loopGen->createLoopPoints();
                loopGen->connectLoops();
         
                //Compute features for each loop
                progressWindow.setStatusMessage("Computing features for loops...");
                for (auto& lp : loopGen->getLoops()) xtractor->computeFeaturesForLoop(lp);
            } else
                progressWindow.threadComplete(true);
            
            return loopGen->getLoops();
        }
        
    }
    
    
} //end namespace