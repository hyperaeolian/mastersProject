/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 29 Jul 2014 10:52:41am
    Author:  milrob

  ==============================================================================
*/


#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(drow::AudioFilePlayerExt& _player) : player(_player), backgroundThread("Waveform Thread"), cache(10),
    thumbnail(512, *player.getAudioFormatManager(), cache)

{
    image = new drow::AudioThumbnailImage(player, backgroundThread, thumbnail, 512);
    
    posDisplay = new drow::PositionableWaveDisplay(*image, backgroundThread);
    addAndMakeVisible(posDisplay);
    
    dragDisplay = new drow::DraggableWaveDisplay(*image);
    addAndMakeVisible(dragDisplay);
    
    backgroundThread.startThread(1);
    
    
}

WaveformDisplay::~WaveformDisplay(){}

void WaveformDisplay::resized(){
    const int w = getWidth();
    //const int h = getHeight();
    int m = 5;
    const int bevelSize = 2;
    
    Rectangle<int> posBounds(0,0,w,50);
    posDisplay->setBounds(posBounds.reduced(bevelSize));
    
    Rectangle<int> dragBounds(0, 50 + m, w, 50);
    dragDisplay->setBounds(dragBounds.reduced(bevelSize));
}

void WaveformDisplay::paint(juce::Graphics &g){
    drow::GuiHelpers::drawBevel(g, posDisplay->getBounds().toFloat(), 2.0f, Colours::coral);
    drow::GuiHelpers::drawBevel(g, dragDisplay->getBounds().toFloat(), 2.0f, Colours::cornflowerblue);
}
