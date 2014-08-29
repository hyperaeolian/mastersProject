/*
  ==============================================================================

   

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
    
    for (int i = 0; i < delimiters.size(); ++i) {
        Loop curr;
        if (delimiters[i] + BarSize <= lastDelimiter) {
            curr.start  = delimiters[i];
            _REAL point = delimiters[i] + BarSize;
            curr.end    = quantizeToDelimiter(point);
            if (curr.start > curr.end) std::swap(curr.start, curr.end);
            if (curr.start * SR > AudioBuffer.size()) continue;
            curr.head = static_cast<int>(curr.start * SR);
            curr.tail = curr.end * SR > AudioBuffer.size() ? AudioBuffer.size() :
                                                             static_cast<int>(curr.end * SR);
            _Loops.push_back(curr);
        } else
            return;
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
    
    
//namespace (non-member) convenience functions
    
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
    std::vector<Loop> constructLoops(const std::vector<_REAL>& buffer){
    
        if (lgen::audioBuffered){
            FeatureExtractor xtractor(buffer);
            //Can use either onsets or beats as loop points
            //xtractor.findOnsets();
            xtractor.findBeats();
            std::vector<std::string> vals = {"Loop Generator", "Initializing", " remaining loops to create",
                                             "Finishing last loop", "You canceled the loop generator",
                                             "Loop Construction Successful!"};
            
            LoopGenerator loopGen(buffer, xtractor.getBeats());
            BackgroundThread progressWindow(static_cast<int>(loopGen.getNumLoopsToCreate()), vals);
            if (progressWindow.runThread()){
                loopGen.createLoopPoints();
                loopGen.connectLoops();
               
                //Compute features for each loop
                for (auto& lp : loopGen.getLoops()) xtractor.computeFeaturesForLoop(lp);
            } else
                progressWindow.threadComplete(true);
            
            return loopGen.getLoops();
        }
        
        //TODO: Handle case where !lgen::audioBuffered
    }
    
    
    
} //end namespace