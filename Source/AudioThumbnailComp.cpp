/*
  ==============================================================================

    AudioThumbnailComp.cpp
    Created: 19 Aug 2014 11:25:28am
    Author:  milrob

  ==============================================================================
*/

#include "JuceHeader.h"

class Waveform :
    public Component,
    public ChangeListener,
    public ChangeBroadcaster,
    private Timer
{
public:
    Waveform(AudioFormatManager& formatManager, AudioTransportSource& _transportSource) :
        transportSource(_transportSource),
        cache(5),
        thumbnail(512, formatManager, cache),
        isFollowingTransport(false)
    {
        thumbnail.addChangeListener(this);
        currentPos.setFill(Colours::white.withAlpha(0.85f));
        addAndMakeVisible(currentPos);
    }
    
    ~Waveform(){
        thumbnail.removeChangeListener(this);
    }
    
    void setFile(const File& file){
        if (!file.isDirectory()) {
            thumbnail.setSource(new FileInputSource(file));
            const Range<double> newRange(0.0, thumbnail.getTotalLength());
            setRange(newRange);
            startTimer(1000/40);
        }
    }
    
    void setZoomFactor(double amount){
        if (thumbnail.getTotalLength() > 0){
            const double newScale = jmax(0.001, thumbnail.getTotalLength() * (1.0 - jlimit(0.0, 0.99, amount)));
            const double timeAtCenter = xToTime(getWidth()/2.0f);
            setRange(Range<double> (timeAtCenter - newScale * 0.5, timeAtCenter + newScale * 0.5));
        }
    }
    
    void setRange(Range<double> newRange){
        visibleRange = newRange;
        updateCursorPosition();
        repaint();
    }
    
    void setFollowsTransport(bool shouldFollow){ isFollowingTransport = shouldFollow; }
    
    void paint(Graphics& g) override {
        g.fillAll(Colours::darkgrey);
        g.setColour(Colours::lightblue);
        if (thumbnail.getTotalLength() > 0.0) {
           // Rectangle<int> thumbArea(getLocalBounds());
            thumbnail.drawChannels(g, getLocalBounds().reduced(4), visibleRange.getStart(), visibleRange.getEnd(), 1.0f);
        } else {
            g.setFont(14.0f);
            g.drawFittedText("No audio file selected", getLocalBounds(), Justification::centred, 2);
        }
    }
    
    void resized() override {}
    
    void changeListenerCallback(ChangeBroadcaster*) override {
        repaint();
    }
    
    
    
    
private:
   // Waveform();
    AudioTransportSource& transportSource;
    
    AudioThumbnailCache cache;
    AudioThumbnail thumbnail;
    Range<double> visibleRange;
    bool isFollowingTransport;
   // File audioFile;
    
    DrawableRectangle currentPos;

    float timeToX(const double time) const {
        return getWidth() * (float) ( (time - visibleRange.getStart()) / (visibleRange.getLength()));
    }
    
    double xToTime(const float x) const {
        return (x / getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
    }
    
    void timerCallback() override {
        setRange(visibleRange.movedToStartAt(transportSource.getCurrentPosition() - (visibleRange.getLength() / 2.0)));
    }
    
    void updateCursorPosition(){
        currentPos.setVisible(transportSource.isPlaying() || isMouseButtonDown());
        currentPos.setRectangle(Rectangle<float>(timeToX(transportSource.getCurrentPosition()) - 1.5f, (float) (getHeight())));
    }
};


