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
    ScopedPointer<drow::PositionableWaveDisplay> posDisplay;
    void resized();
    void paint(Graphics& g);

private:
    drow::AudioFilePlayerExt& player;
    drow::TimeSliceThread backgroundThread;
    drow::AudioThumbnailCache cache;
    drow::ColouredAudioThumbnail thumbnail;
    
    ScopedPointer<drow::AudioThumbnailImage> image;
    
    ScopedPointer<drow::DraggableWaveDisplay> dragDisplay;
    
};
#endif  // WAVEFORMDISPLAY_H_INCLUDED
