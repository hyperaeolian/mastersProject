/*
  ==============================================================================

    AudioWaveform.h
    Created: 29 Aug 2014 11:36:21am
    Author:  milrob

  ==============================================================================
*/

#ifndef AUDIOWAVEFORM_H_INCLUDED
#define AUDIOWAVEFORM_H_INCLUDED

#include "JuceHeader.h"

class Waveform :
public Component,
public ChangeListener,
public ChangeBroadcaster,
private ScrollBar::Listener,
private Timer
{
    
public:
    explicit Waveform(drow::AudioFilePlayerExt& afp);
    ~Waveform();
    
    void setFile(const File& file);
    void setZoomFactor(double amount);
    
    void setRange(Range<double> newRange);
    
    void setFollowsTransport(bool shouldFollow){ isFollowingTransport = shouldFollow; }
    
    void paint(Graphics& g) override;
    void resized() override { scrollbar.setBounds(getLocalBounds().removeFromBottom(14).reduced(2)); }

    void changeListenerCallback(ChangeBroadcaster*) override { repaint(); }

    void isShiftyLooping(bool shouldBeShifty){ shifty = shouldBeShifty; }

    void setEndTime(float end){ endTime = end; }


private:
    drow::AudioFilePlayerExt& audioFilePlayer;
    ScrollBar scrollbar;
    AudioThumbnailCache cache;
    AudioThumbnail thumbnail;
    Range<double> visibleRange;
    bool isFollowingTransport, shifty;
    float endTime;


    DrawableRectangle currentPos, endPos;

    float timeToX(const double time) const {
        return getWidth() * (float) ( (time - visibleRange.getStart()) / (visibleRange.getLength()));
    }

    double xToTime(const float x) const {
        return (x / getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
    }

    void scrollBarMoved(ScrollBar* scrollBarThatHasMoved, double newRangeStart) override;

    bool canMoveTransport() const noexcept {
        return !(isFollowingTransport && audioFilePlayer.isPlaying());
    }

    void timerCallback() override;

    void updateCursorPosition();
};




#endif  // AUDIOWAVEFORM_H_INCLUDED
