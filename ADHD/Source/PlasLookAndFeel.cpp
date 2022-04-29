/*
 ==============================================================================
 
 PlasLookAndFeel.cpp
 Created: 12 Apr 2022 3:32:30pm
 Author:  Pox17
 
 ==============================================================================
 */

#include "PlasLookAndFeel.h"
#include <BinaryData.cpp>

PLASLookAndFeel::PLASLookAndFeel() 
{
    knobStripN = juce::Image();
    knobStripD = juce::Image();
    frames = 0;
    destroy = false;
}

void PLASLookAndFeel::setKnobStripN(juce::Image src) {
    knobStripN = src;
}
void PLASLookAndFeel::setKnobStripD(juce::Image src) {
    knobStripD = src;
}
void PLASLookAndFeel::setFrames(int fr) {
    frames = fr;
}
void PLASLookAndFeel::setDestroy(bool dest) {
    destroy = dest;
}
void PLASLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                       const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    juce::Image knobStrip = juce::Image();
    
    if(destroy) {
        knobStrip = knobStripD;
    }
    else {
        knobStrip = knobStripN;
    }
    float knobDimW = width;
    float knobDimH = height;
    float frameDimW = knobStrip.getWidth();
    float frameDimH = knobStrip.getHeight() / frames;
    if ((knobDimW / frameDimW) <= (knobDimH / frameDimH)) {
        knobDimH = (knobDimW / frameDimW) * frameDimH;
        knobDimW = (knobDimW / frameDimW) * frameDimW;
    }
    else {
        knobDimH = (knobDimH / frameDimH) * frameDimH;
        knobDimW = (knobDimH / frameDimH) * frameDimW;
    }
    const double rotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum());
    const int frameId = (int)ceil(rotation * ((double)frames - 1.0));
    int centerX = width / 2;
    int centerY = height / 2;
    int sX = centerX - (knobDimW / 2);
    int sY = centerY - (knobDimH / 2);
    g.drawImage(knobStrip, sX, sY, knobDimW, knobDimH, 0, frameId * frameDimH, frameDimW, frameDimH);
}

/*
 knobs LookAndFeel implementation
 */

PLASBigKnobLookFeel::PLASBigKnobLookFeel() {
    PLASLookAndFeel::setKnobStripD(juce::ImageCache::getFromMemory(BinaryData::Primary_Destroyed_png, BinaryData::Primary_Destroyed_pngSize));
    PLASLookAndFeel::setKnobStripN(juce::ImageCache::getFromMemory(BinaryData::Primary_png, BinaryData::Primary_pngSize));
    PLASLookAndFeel::setFrames(128);
}

PLASLittleKnobLookFeel::PLASLittleKnobLookFeel() {
    PLASLookAndFeel::setKnobStripD(juce::ImageCache::getFromMemory(BinaryData::Secondary_Destroyed_png, BinaryData::Secondary_Destroyed_pngSize));
    PLASLookAndFeel::setKnobStripN(juce::ImageCache::getFromMemory(BinaryData::Secondary_png, BinaryData::Secondary_pngSize));
    PLASLookAndFeel::setFrames(128);
}


PLASDestroyButtonLookAndFeel::PLASDestroyButtonLookAndFeel() {
    buttonOff = juce::ImageCache::getFromMemory(BinaryData::dButtonOff_png, BinaryData::dButtonOff_pngSize);
    buttonOn = juce::ImageCache::getFromMemory(BinaryData::dButtonOn_png, BinaryData::dButtonOn_pngSize);
}

void PLASDestroyButtonLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    juce::Image btn = juce::Image();
    
    if (button.getToggleState()) {
        btn = buttonOn;
    }
    else {
        btn = buttonOff;
    }
    float btDimW = (float)button.getWidth();
    float btDimH = (float)button.getHeight();
    float frameDimW = (float)btn.getWidth();
    float frameDimH = (float)btn.getHeight();
    
    if ((btDimW/frameDimW) <= ( btDimH / frameDimH)) {
        btDimH = (btDimW / frameDimW) * frameDimH;
        btDimW = (btDimW / frameDimW) * frameDimW;
    }
    else {
        btDimH = (btDimH / frameDimH) * frameDimH;
        btDimW = (btDimH / frameDimH) * frameDimW;
    }
    int centerX = (button.getWidth()) / 2;
    int centerY = (button.getHeight()) / 2;
    int sX = centerX - (btDimW / 2);
    int sY = centerY - (btDimH / 2);
    g.drawImage(btn, sX, sY, btDimW, btDimH, 0, 0, frameDimW, frameDimH);
    
}

//PLASButtonLookAndFeel

PLASButtonLookAndFeel::PLASButtonLookAndFeel(juce::Image abuttonOnN,
                      juce::Image abuttonOffN,
                      juce::Image abuttonOnD,
                      juce::Image abuttonOffD,
                      bool destroy){
    buttonOnN = abuttonOnN;
    buttonOffN = abuttonOffN;
    buttonOnD = abuttonOnD;
    buttonOffD = abuttonOffD; 
    destroy = destroy;
}


void PLASButtonLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    juce::Image btn = juce::Image();
    if(!destroy) {
        if (button.getToggleState()) {
            btn = buttonOnN;
        }
        else {
            btn = buttonOffN;
        }
    } else {
        if (button.getToggleState()) {
            btn = buttonOnD;
        }
        else {
            btn = buttonOffD;
        }
    }
    float btDimW = (float)button.getWidth();
    float btDimH = (float)button.getHeight();
    float frameDimW = (float)btn.getWidth();
    float frameDimH = (float)btn.getHeight();
    
    if ((btDimW/frameDimW) <= ( btDimH / frameDimH)) {
        btDimH = (btDimW / frameDimW) * frameDimH;
        btDimW = (btDimW / frameDimW) * frameDimW;
    }
    else {
        btDimH = (btDimH / frameDimH) * frameDimH;
        btDimW = (btDimH / frameDimH) * frameDimW;
    }
    int centerX = (button.getWidth()) / 2;
    int centerY = (button.getHeight()) / 2;
    int sX = centerX - (btDimW / 2);
    int sY = centerY - (btDimH / 2);
    g.drawImage(btn, sX, sY, btDimW, btDimH, 0, 0, frameDimW, frameDimH);
    
}


PLASButLinkLookAndFeel::PLASButLinkLookAndFeel(): PLASButtonLookAndFeel(juce::ImageCache::getFromMemory(BinaryData::Link_Button_png, BinaryData::Link_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Unlink_Button_png, BinaryData::Unlink_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Destroyed_Link_Button_png, BinaryData::Destroyed_Link_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Unlink_Button_png, BinaryData::Unlink_Button_pngSize), false)
{
}
PLASButOnLookAndFeel::PLASButOnLookAndFeel(): PLASButtonLookAndFeel(juce::ImageCache::getFromMemory(BinaryData::ON_Button_png, BinaryData::ON_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_Button_png, BinaryData::OFF_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Destroyed_ON_Button_png, BinaryData::Destroyed_ON_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_Button_png, BinaryData::OFF_Button_pngSize),  false)
{
}
PLASButBPLookAndFeel::PLASButBPLookAndFeel(): PLASButtonLookAndFeel(juce::ImageCache::getFromMemory(BinaryData::ON_BP_Button_png, BinaryData::ON_BP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_BP_Button_png, BinaryData::OFF_BP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Destroyed_ON_BP_Button_png, BinaryData::Destroyed_ON_BP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_Button_png, BinaryData::OFF_BP_Button_pngSize),  false)
{
}
PLASButLPLookAndFeel::PLASButLPLookAndFeel(): PLASButtonLookAndFeel(juce::ImageCache::getFromMemory(BinaryData::ON_LP_Button_png, BinaryData::ON_LP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_LP_Button_png, BinaryData::OFF_LP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Destroyed_ON_LP_Button_png, BinaryData::Destroyed_ON_LP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_LP_Button_png, BinaryData::OFF_LP_Button_pngSize),  false)
{
}
PLASButHPLookAndFeel::PLASButHPLookAndFeel(): PLASButtonLookAndFeel(juce::ImageCache::getFromMemory(BinaryData::ON_HP_Button_png, BinaryData::ON_HP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_HP_Button_png, BinaryData::OFF_HP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Destroyed_ON_HP_Button_png, BinaryData::Destroyed_ON_HP_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_HP_Button_png, BinaryData::OFF_HP_Button_pngSize),  false)
{
}
PLASButMSLookAndFeel::PLASButMSLookAndFeel(): PLASButtonLookAndFeel(juce::ImageCache::getFromMemory(BinaryData::MS_Button_png, BinaryData::MS_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_MS_Button_png, BinaryData::OFF_MS_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Destroyed_MS_Button_png, BinaryData::Destroyed_MS_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_MS_Button_png, BinaryData::OFF_MS_Button_pngSize),  false)
{
}
PLASButLRLookAndFeel::PLASButLRLookAndFeel(): PLASButtonLookAndFeel(juce::ImageCache::getFromMemory(BinaryData::LR_Button_png, BinaryData::LR_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_LR_Button_png, BinaryData::OFF_LR_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::Destroyed_LR_Button_png, BinaryData::Destroyed_LR_Button_pngSize), juce::ImageCache::getFromMemory(BinaryData::OFF_LR_Button_png, BinaryData::OFF_LR_Button_pngSize),  false)
{
}
