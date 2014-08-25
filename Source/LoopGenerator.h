/*
  ==============================================================================

    LoopGen.h
    Created: 16 Jul 2014 2:23:14pm
    Author:  milrob

  ==============================================================================
*/

#ifndef LOOPGEN_H_INCLUDED
#define LOOPGEN_H_INCLUDED

#define _REAL essentia::Real

#include <vector>
#include "JuceHeader.h"
#include "pool.h"
#include "essentia.h"
#include "algorithmfactory.h"
#include "ProgressWindow.h"



//extern const int SR;
//extern const float BAR_SIZE;


struct Loop {
    float start, end;
    int head, tail;
    int numBeats;
    Loop* prev;
    Loop* next;
    essentia::Pool bin;
    essentia::Pool binStats;
};

namespace lgen{

class LoopGenerator {
public:
    LoopGenerator(const std::vector<_REAL>& _AudioBuffer, const std::vector<_REAL>& _delim);
    ~LoopGenerator();
    
    void createLoopPoints();
    void connectLoops();
    _REAL quantizeToDelimiter(_REAL value);
    
    std::vector<_REAL> getBuffer() const { return AudioBuffer; }
    std::vector<Loop>  getLoops()  const { return _Loops; }
    _REAL getNumLoopsToCreate()    const { return delimiters.size(); }
    
    void setLastDelimiter(std::vector<_REAL>& dv){ lastDelimiter = dv.back(); }
        
private:
    const int SR = 44100;
    const float BarSize;
    float lastDelimiter;
    const std::vector<_REAL> AudioBuffer;
    std::vector<_REAL>       delimiters;
    std::vector<Loop>        _Loops;
    
    LoopGenerator(const LoopGenerator&);
    LoopGenerator(LoopGenerator&&);
    LoopGenerator& operator=(const LoopGenerator&);
    LoopGenerator& operator=(LoopGenerator&&);
};
    
    
    std::vector<_REAL> AudioBuffer;
    void initAudio(const std::string audiofilename);
    std::vector<Loop> computeLoops();
}

/*
 std::vector<Loop> computeLoops(const std::string audiofilename, essentia::Real& bpm);
 void createLoopPoints(const std::vector<float>& onsets, const std::vector<essentia::Real>& AUDIO_BUFFER, std::vector<Loop>& loops);
 void connectLoops(std::vector<Loop>& loops);
 //void findOverlaps(std::vector<Loop>& loops);
 float quantizeToOnset(const std::vector<float>& onsets, float value);
 //void printBuffer(Loop& a);
 */


#endif  // LOOPGEN_H_INCLUDED
