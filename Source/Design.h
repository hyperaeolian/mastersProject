/*
  ==============================================================================

    Designn.h
    Created: 29 Aug 2014 11:37:47am
    Author:  milrob

  ==============================================================================
*/

#ifndef DESIGNN_H_INCLUDED
#define DESIGNN_H_INCLUDED


#include "JuceHeader.h"

class CustomLookAndFeel : public LookAndFeel_V3{
public:
    CustomLookAndFeel(const char* _img, const int _imgSize) :
    knobImg(_img), knobImgSize(_imgSize)
    {}
    
    ~CustomLookAndFeel(){
        knobImg = nullptr;
    }
    
    void drawRoundThumb (Graphics& g, const float x, const float y,
                         const float diameter, const Colour& colour, float outlineThickness);
    
    void drawButtonBackground (Graphics& g, Button& button, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override;
    
    void drawTickBox (Graphics& g, Component& component,
                      float x, float y, float w, float h,
                      bool ticked,
                      bool isEnabled,
                      bool isMouseOverButton,
                      bool isButtonDown) override;
    
    void drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
                                float sliderPos, float minSliderPos, float maxSliderPos,
                                const Slider::SliderStyle style, Slider& slider) override;
    
    void drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const Slider::SliderStyle style, Slider& slider) override;
    
    void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
                                     float /*sliderPos*/,
                                     float /*minSliderPos*/,
                                     float /*maxSliderPos*/,
                                     const Slider::SliderStyle /*style*/, Slider& slider) override;
    
    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                           float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;
    
private:
    const char* knobImg;
    const int knobImgSize;
};




#endif  // DESIGNN_H_INCLUDED
