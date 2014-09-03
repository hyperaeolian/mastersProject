/*
  ==============================================================================

    ShiftyLooping.h
    Created: 27 Aug 2014 11:42:28am
    Author:  milrob

  ==============================================================================
*/

#ifndef SHIFTYLOOPING_H_INCLUDED
#define SHIFTYLOOPING_H_INCLUDED

#include "JuceHeader.h"
#include "LoopGenerator.h"


class ShiftyLooper : public drow::AudioFilePlayerExt,
                     public drow::AudioFilePlayer::Listener,
    public AudioIODeviceCallback

{
public:
    ShiftyLooper();
    ~ShiftyLooper();
    

    //AudioIODevice
    void audioDeviceIOCallback (const float **inputChannelData, int numInputChannels, float **outputChannelData, int numOutputChannels, int numSamples) override;
    void audioDeviceAboutToStart (AudioIODevice *device){}
    void audioDeviceStopped(){}

    //Modifiers
    void setLoops(const std::vector<Loop>& l)     { _Loops = l; }
    void setMarkovChain(const std::vector<int> s) { markovChain = s; }
    void setShiftyLooping(bool sl)                { shouldShiftyLoop = sl; }
    
    void shiftyLooping();
    
    bool requestMarkovUpdate() const {return updateMarkov;}
    
    //drow
    void fileChanged(drow::AudioFilePlayer* player) {}
    void audioFilePlayerSettingChanged(drow::AudioFilePlayer* player, int settingCode){}
    void playerStoppedOrStarted(drow::AudioFilePlayer* player){}

private:
    std::vector<Loop> _Loops;
    Loop* currentLoop;
    std::vector<int> markovChain;
    int index;
    bool shifting, forward, shouldShiftyLoop, updateMarkov;
    
    ShiftyLooper(const ShiftyLooper&);
    ShiftyLooper(ShiftyLooper&&);
    ShiftyLooper& operator=(const ShiftyLooper&);
    ShiftyLooper& operator=(ShiftyLooper&&);
    
    void getNextDirection();
    void updateCurrentLoop();

};


#endif  // SHIFTYLOOPING_H_INCLUDED
