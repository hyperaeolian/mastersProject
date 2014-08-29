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


class ShiftyLooper : public drow::AudioFilePlayer::Listener,
                     public AudioIODeviceCallback
{
public:
    ShiftyLooper(drow::AudioFilePlayerExt& afp);
    
    ~ShiftyLooper();
    
    //AudioIODevice
    void audioDeviceIOCallback (const float **inputChannelData, int numInputChannels, float **outputChannelData, int numOutputChannels, int numSamples);

    void audioDeviceAboutToStart (AudioIODevice *device){}
    void audioDeviceStopped(){}

    void updateSequence(std::vector<int> seq){sequence = seq; }
    
    //drow
    void fileChanged(drow::AudioFilePlayer* player) {}
    void audioFilePlayerSettingChanged(drow::AudioFilePlayer* player, int settingCode){}
    void playerStoppedOrStarted(drow::AudioFilePlayer* player){}

private:
    drow::AudioFilePlayerExt& player;
  //  std::vector<Loop> _Loops;
    std::vector<int> sequence;
    
    ShiftyLooper(const ShiftyLooper&);
    ShiftyLooper(ShiftyLooper&&);
    ShiftyLooper& operator=(const ShiftyLooper&);
    ShiftyLooper& operator=(ShiftyLooper&&);

};


#endif  // SHIFTYLOOPING_H_INCLUDED
