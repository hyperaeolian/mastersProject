/*
  ==============================================================================

    WaveformDisplay.h
    Created: 29 Jul 2014 10:52:41am
    Author:  milrob

  ==============================================================================
*/

#ifndef WAVEFORMDISPLAY_H_INCLUDED
#define WAVEFORMDISPLAY_H_INCLUDED

#include "JuceHeader.h"

class WaveformDisplay : public Component {
public:
    WaveformDisplay(drow::AudioFilePlayerExt& _player);
    ~WaveformDisplay();
    void resized();
    void paint(Graphics& g);

private:
    drow::AudioFilePlayerExt& player;
    drow::TimeSliceThread backgroundThread;
    drow::AudioThumbnailCache cache;
    drow::ColouredAudioThumbnail thumbnail;
    
    ScopedPointer<drow::AudioThumbnailImage> image;
    ScopedPointer<drow::PositionableWaveDisplay> posDisplay;
    ScopedPointer<drow::DraggableWaveDisplay> dragDisplay;
   
    
    void* foo;
};
#endif  // WAVEFORMDISPLAY_H_INCLUDED
