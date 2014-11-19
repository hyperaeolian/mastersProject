/*
  ==============================================================================

    LoopGen.h
    Created: 16 Jul 2014 2:23:14pm
    Author:  milrob

  ==============================================================================
*/

#ifndef LOOPGEN_H_INCLUDED
#define LOOPGEN_H_INCLUDED

#include <vector>
#include "JuceHeader.h"
#include "pool.h"
#include "essentia.h"
#include "algorithmfactory.h"
#include "ProgressWindow.h"

typedef essentia::Real _REAL;
typedef std::vector<essentia::Real> VEC_REAL;

struct Loop {
    //float start, end;
    _REAL start, end;
    int sampsStart, sampsEnd;
    
    Loop* prev;
    Loop* next;
    essentia::Pool bin;
    
    Loop operator++(int){
        *this = *this->next;
        return *this;
    }
    
    Loop operator--(int){
        *this = *this->prev;
        return *this;
    }
    
    
    
};

namespace lgen{

class LoopGenerator {
public:
    LoopGenerator(const VEC_REAL& _AudioBuffer, const VEC_REAL& _delim);
    ~LoopGenerator();
    
    void createLoopPoints();
    void connectLoops();
    _REAL quantizeToDelimiter(_REAL value);
    
    VEC_REAL           getBuffer() const { return AudioBuffer; }
    std::vector<Loop>  getLoops()  const { return _Loops; }
    _REAL getNumLoopsToCreate()    const { return delimiters.size(); }
    
    void setLastDelimiter(VEC_REAL& dv){ lastDelimiter = dv.back(); }
        
private:
    const int SR = 44100;
    const float BarSize;
    float lastDelimiter;
    const VEC_REAL AudioBuffer;
    VEC_REAL       delimiters;
    std::vector<Loop>        _Loops;
    
    LoopGenerator(const LoopGenerator&);
    LoopGenerator(LoopGenerator&&);
    LoopGenerator& operator=(const LoopGenerator&);
    LoopGenerator& operator=(LoopGenerator&&);
};
    
//namespace (non-member) helper functions
    
    VEC_REAL initAudio(const std::string audiofilename);
    std::vector<Loop> constructLoops(const VEC_REAL& buffer);
    
}



#endif  // LOOPGEN_H_INCLUDED
