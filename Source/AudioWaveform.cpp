/*
  ==============================================================================

    AudioThumbnailComp.cpp
    Created: 19 Aug 2014 11:25:28am
    Author:  milrob

  ==============================================================================
*/

#include "AudioWaveform.h"



Waveform::Waveform(drow::AudioFilePlayerExt& afp) :
        audioFilePlayer(afp),
        scrollbar(false),
        cache(10),
        thumbnail(512, *audioFilePlayer.getAudioFormatManager(), cache)
    {
        thumbnail.addChangeListener(this);
        addAndMakeVisible(scrollbar);
        scrollbar.setRangeLimits(visibleRange);
        scrollbar.setAutoHide(false);
        scrollbar.addListener(this);
        currentPos.setFill(Colours::white.withAlpha(0.85f));
        endPos.setFill(Colours::white.withAlpha(0.85f));
        addAndMakeVisible(currentPos);
        addAndMakeVisible(endPos);
        endTime = 0.0f;
        isFollowingTransport = shifty = false;
    }
    
Waveform::~Waveform(){
        scrollbar.removeListener(this);
        thumbnail.removeChangeListener(this);
    }
    
void Waveform::setFile(const File& file){
        if (!file.isDirectory()) {
            thumbnail.setSource(new FileInputSource(file));
            const Range<double> newRange(0.0, thumbnail.getTotalLength());
            scrollbar.setRangeLimits(newRange);
            setRange(newRange);
            startTimer(1000/40);
        }
    }
    
void Waveform::setZoomFactor(double amount){
        if (thumbnail.getTotalLength() > 0){
            const double newScale = jmax(0.001, thumbnail.getTotalLength() * (1.0 - jlimit(0.0, 0.99, amount)));
            const double timeAtCenter = xToTime(getWidth()/2.0f);
            setRange(Range<double> (timeAtCenter - newScale * 0.5, timeAtCenter + newScale * 0.5));
        }
    }
    
void Waveform::setRange(Range<double> newRange){
        visibleRange = newRange;
        scrollbar.setCurrentRange(visibleRange);
        updateCursorPosition();
        repaint();
    }

    
void Waveform::paint(Graphics& g){
        g.fillAll(Colours::darkgrey);
        g.setColour(Colours::lightblue);
        if (thumbnail.getTotalLength() > 0.0) {
            Rectangle<int> thumbArea(getLocalBounds());
            thumbArea.removeFromBottom(scrollbar.getHeight() + 4);
            thumbnail.drawChannels(g, thumbArea.reduced(4), visibleRange.getStart(), visibleRange.getEnd(), 1.0f);
        } else {
            g.setFont(14.0f);
            g.drawFittedText("No audio file selected", getLocalBounds(), Justification::centred, 2);
        }
    }


void Waveform::scrollBarMoved(ScrollBar* scrollBarThatHasMoved, double newRangeStart){
        if (scrollBarThatHasMoved == &scrollbar)
            if (!(isFollowingTransport && audioFilePlayer.isPlaying()))
                setRange(visibleRange.movedToStartAt(newRangeStart));
    }

void Waveform::timerCallback(){
        if (canMoveTransport())
            updateCursorPosition();
        else{
            setRange(visibleRange.movedToStartAt(audioFilePlayer.getCurrentPosition() - (visibleRange.getLength() / 2.0)));
        }
    }
    
void Waveform::updateCursorPosition(){
        if (shifty){
            currentPos.setVisible(audioFilePlayer.isPlaying() || isMouseButtonDown());
            endPos.setVisible(audioFilePlayer.isPlaying() || audioFilePlayer.getLoopBetweenTimes());
            currentPos.setRectangle(Rectangle<float> (timeToX(audioFilePlayer.getCurrentPosition()) - 0.75f,
                                                      0, 1.5f, (float) (getHeight() - scrollbar.getHeight())));
            endPos.setRectangle(Rectangle<float>(timeToX(audioFilePlayer.getCurrentPosition() + endTime) - 0.75f,
                                                 0, 1.5f, (float) (getHeight() - scrollbar.getHeight())));
        } else {
            currentPos.setVisible(audioFilePlayer.isPlaying() || isMouseButtonDown());
            currentPos.setRectangle(Rectangle<float> (timeToX(audioFilePlayer.getCurrentPosition()) - 0.75f,
                                                      0, 2.5f, (float) (getHeight() - scrollbar.getHeight())));
        }
    }
