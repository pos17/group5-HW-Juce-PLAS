/*
  ==============================================================================

    ChannelComponent.cpp
    Created: 19 Apr 2022 3:56:17pm
    Author:  stefa

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ChannelComponent.h"

//==============================================================================
ChannelComponent::ChannelComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    inGainDial.setRange(0.0, 1.0, 1. / 128);
    inGainDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    inGainDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    outGainDial.setRange(0.0, 1.0, 1. / 128);
    outGainDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    outGainDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    clipDial.setRange(0.0, 1.0, 1. / 128);
    clipDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    clipDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    toneDial.setRange(0.0, 1.0, 1. / 128);
    toneDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    toneDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    freqDial.setRange(0.0, 1.0, 1. / 128);
    freqDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    freqDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    filterOn.setToggleState(true, false);

    //ADD AND MAKE VISIBLE
    addAndMakeVisible(inGainDial);
    addAndMakeVisible(outGainDial);
    addAndMakeVisible(clipDial);
    addAndMakeVisible(toneDial);
    addAndMakeVisible(freqDial);

    addAndMakeVisible(filterOn);
    addAndMakeVisible(filterLP);
    addAndMakeVisible(filterBP);
    addAndMakeVisible(filterHP);
}

ChannelComponent::~ChannelComponent()
{
}

void ChannelComponent::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::red);
    g.drawRect(getLocalBounds());
}

void ChannelComponent::resized()
{
    const int wL = getWidth();
    const int hL = getHeight();
    //const int xL = getX();
    //const int yL = getY();
    DBG("wL: " << wL <<"\nhL: " << hL);

    juce::Rectangle<int> filterContainerL(0, 0, wL / 4, hL);
    juce::Rectangle<int> dialsContainerL(filterContainerL.getWidth(), 0, 3 * wL / 4, hL);
    
    // Input Filter
    const int dim = 30; //px
    const int wFL = filterContainerL.getWidth();
    const int hFL = filterContainerL.getHeight();
    const int xFL0 = filterContainerL.getX();
    const int yFL = filterContainerL.getY() + (dim / 2);

    int xFL = (wFL - (4 * dim)) / 8;
    
    juce::Rectangle<int> filterOnAreaL(xFL0 + xFL, yFL, dim, dim);
    juce::Rectangle<int> filterLPAreaL(xFL0 + (3 * xFL) + dim, yFL, dim, dim);
    juce::Rectangle<int> filterBPAreaL(xFL0 + (5 * xFL) + (2 * dim), yFL, dim, dim);
    juce::Rectangle<int> filterHPAreaL(xFL0 + (7 * xFL) + (3 * dim), yFL, dim, dim);

    juce::Rectangle<int> freqDialAreaL(xFL0, yFL - (dim / 2) + (hFL / 3), wFL, hFL * 2 / 3);

    // Main Dials
    const int wDL = dialsContainerL.getWidth();
    const int hDL = dialsContainerL.getHeight();
    const int xDL = dialsContainerL.getX();
    const int yDL = dialsContainerL.getY();

    juce::Rectangle<int> inGainDialAreaL(xDL, yDL, wDL / 3, hDL);
    juce::Rectangle<int> outGainDialAreaL(xDL + (2 * wDL / 3), yDL, wDL / 3, hDL);
    juce::Rectangle<int> clipDialAreaL(xDL + (wDL / 3), yDL + (hDL / 4), wDL / 6, hDL / 2);
    juce::Rectangle<int> toneDialAreaL(xDL + (wDL / 2), yDL + (hDL / 4), wDL / 6, hDL / 2);


    //SETTING BOUNDS
    inGainDial.setBounds(inGainDialAreaL);
    outGainDial.setBounds(outGainDialAreaL);
    clipDial.setBounds(clipDialAreaL);
    toneDial.setBounds(toneDialAreaL);
    freqDial.setBounds(freqDialAreaL);

    filterOn.setBounds(filterOnAreaL);
    filterLP.setBounds(filterLPAreaL);
    filterBP.setBounds(filterBPAreaL);
    filterHP.setBounds(filterHPAreaL);
}
