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
    PLASLookAndFeel::setKnobStripD(juce::ImageCache::getFromMemory(BinaryData::bigKnobD_png, BinaryData::bigKnobD_pngSize));
    PLASLookAndFeel::setKnobStripN(juce::ImageCache::getFromMemory(BinaryData::bigKnobN_png, BinaryData::bigKnobN_pngSize));
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
    /*
    auto font = getTextButtonFont(button, button.getHeight());
    g.setFont(font);
    g.setColour(button.findColour(button.getToggleState() ? juce::TextButton::textColourOnId
        : juce::TextButton::textColourOffId)
        .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

    auto yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
    auto cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;

    auto fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
    auto leftIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    auto rightIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
    auto textWidth = button.getWidth() - leftIndent - rightIndent;

    if (textWidth > 0)
        g.drawFittedText(button.getButtonText(),
            leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
            juce::Justification::centred, 2);
            */
}
