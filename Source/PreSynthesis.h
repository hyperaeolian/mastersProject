/*
  ==============================================================================

    PreSynthesis.h
    Created: 31 Jul 2014 11:31:34am
    Author:  milrob

  ==============================================================================
*/

#ifndef PRESYNTHESIS_H_INCLUDED
#define PRESYNTHESIS_H_INCLUDED

#include "AudioApp.h"

void compute_SDM(std::vector<Loop> loops);

template <typename T>
T eucNorm(T t1, T t2){ return sqrt(pow((t2 - t1),(T)2)); }

int NoteNameToOrdVal(char s){
    char alpha[] = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"};
    int itr = 0;
    while (s != alpha[itr]) itr++;
    return itr;
}

#endif  // PRESYNTHESIS_H_INCLUDED
