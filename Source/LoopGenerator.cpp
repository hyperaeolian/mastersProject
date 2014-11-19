/*
  ==============================================================================

   

  ==============================================================================
*/

#include "LoopGenerator.h"
#include "FeatureExtractor.h"


namespace lgen{

LoopGenerator::LoopGenerator(const VEC_REAL& _AudioBuffer, const VEC_REAL& _delim) :
    BarSize(1.0), AudioBuffer(_AudioBuffer), delimiters(_delim)
{
    lastDelimiter = delimiters.back();
}

LoopGenerator::~LoopGenerator(){}

void LoopGenerator::createLoopPoints(){
    
    for (int i = 0; i < delimiters.size(); ++i) {
        Loop curr; //redeclare each time or just change values?
        if (delimiters[i] + BarSize <= lastDelimiter) {
            curr.start  = delimiters[i];
            _REAL point = delimiters[i] + BarSize;
            curr.end    = quantizeToDelimiter(point);
            if (curr.start > curr.end)
                std::swap(curr.start, curr.end);
            if (curr.start * SR > AudioBuffer.size())
                continue;
            curr.sampsStart = static_cast<int>(curr.start * SR);
            curr.sampsEnd = curr.end * SR > AudioBuffer.size() ? AudioBuffer.size() :
                                                             static_cast<int>(curr.end * SR);
            _Loops.push_back(curr);
        } else
            return;
    }
}

void LoopGenerator::connectLoops(){
    for (int i = 0; i < _Loops.size(); ++i) {
        if (i == 0) {
            //_Loops[i].prev = &_Loops[i];
            _Loops[i].prev = nullptr;
            _Loops[i].next = &_Loops[1];
        } else if (i == _Loops.size() - 1){
            _Loops[i].prev = &_Loops[i-1];
            _Loops[i].next = nullptr;
            //_Loops[i].next = &_Loops[i];
        } else {
            _Loops[i].prev = &_Loops[i-1];
            _Loops[i].next = &_Loops[i+1];
        }
    }
}

_REAL LoopGenerator::quantizeToDelimiter(_REAL value){
    if (value > lastDelimiter) return lastDelimiter;
    
    
    auto limit = std::equal_range(delimiters.begin(), delimiters.end(), value);
    _REAL diff1 = abs(value - delimiters[limit.first - delimiters.begin()]);
    _REAL diff2 = abs(delimiters[limit.second - delimiters.begin()] - value);
    
    if (diff1 > diff2)
        return delimiters[limit.second - delimiters.begin()];
    else
        return delimiters[limit.first - delimiters.begin()];
}
    
    
//namespace (non-member) convenience functions
    
    bool audioBuffered;
    _REAL bpm;
    
//==========================Buffer Audio========================================
    VEC_REAL initAudio(const std::string audiofilename){
        essentia::init();
        juce::ScopedPointer<essentia::standard::Algorithm>
            loader = essentia::standard::AlgorithmFactory::create("MonoLoader",
                                                                  "filename", audiofilename,
                                                                  "sampleRate", 44100);
        VEC_REAL buffer;
        loader->output("audio").set(buffer);
        loader->compute();
        lgen::audioBuffered = true;
        essentia::shutdown();
        
        return buffer;
       
    }
    
//==========================Compute Loops=======================================
    std::vector<Loop> constructLoops(const VEC_REAL& buffer){

        if (lgen::audioBuffered){
            FeatureExtractor xtractor(buffer);
            //Can use either onsets or beats as loop points
            //xtractor.findOnsets();
            //xtractor.findBeats();
            xtractor.computeFeaturesForBuffer();
            LoopGenerator loopGen(buffer, xtractor.getBeats());
            
            loopGen.createLoopPoints();
            loopGen.connectLoops();
            
            for (auto& lp : loopGen.getLoops())
                xtractor.computeFeaturesForLoop(lp);

            return loopGen.getLoops();
        } else {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon,"Audio File",
                                             "Unable to buffer audiofile");
            //TODO: Handle case where !lgen::audioBuffered with alert window
            //Redesign this function by returning value via ptr parameter
            //http://stackoverflow.com/questions/3227755/return-nothing-from-non-void-function-in-c
        }
        
    }
    
    
    
} //end namespace