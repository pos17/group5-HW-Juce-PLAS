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
    freqDial.setRange(0.0, 1.0, 1. / 128);
    freqDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    freqDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    qFactorDial.setRange(0.0, 1.0, 1. / 128);
    qFactorDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    qFactorDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    inGainDial.setRange(0.0, 1.0, 1. / 128);
    inGainDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    inGainDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    toneDial.setRange(0.0, 1.0, 1. / 128);
    toneDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    toneDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    outGainDial.setRange(0.0, 1.0, 1. / 128);
    outGainDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    outGainDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    drywetDial.setRange(0.0, 1.0, 1. / 128);
    drywetDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    drywetDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    channelOn.setToggleState(true, false);
    filterOn.setToggleState(true, false);

    //ADD AND MAKE VISIBLE
    addAndMakeVisible(freqDial);

    addAndMakeVisible(inGainDial);
    addAndMakeVisible(outGainDial);
    addAndMakeVisible(qFactorDial);
    addAndMakeVisible(toneDial);

    addAndMakeVisible(drywetDial);


    addAndMakeVisible(channelOn);

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
    const int buttonDimension = 30; //px
    const int primaryDialDimension = 120; //px
    const int secondaryDialDimension = 80; //px

    //Buttons Bounds
    juce::Rectangle<int> channelOnArea(20, 65, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterOnArea(123, 20, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterLPArea(157, 20, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterBPArea(193, 20, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterHPArea(227, 20, buttonDimension, buttonDimension);

    //Dials Bounds
    juce::Rectangle<int> freqDialArea(109, 62, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> qFactorDialArea(189, 62, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> inGainDialArea(292, 20, primaryDialDimension, primaryDialDimension);
    juce::Rectangle<int> toneDialArea(428, 40, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> outGainDialArea(524, 20, primaryDialDimension, primaryDialDimension);
    juce::Rectangle<int> drywetDialArea(660, 40, secondaryDialDimension, secondaryDialDimension);

    g.setColour(juce::Colours::aquamarine);
    g.drawRect(inGainDialArea);
    g.drawRect(outGainDialArea);
    g.drawRect(toneDialArea);
    g.drawRect(drywetDialArea);
    g.drawRect(channelOnArea);

    g.setColour(juce::Colours::pink);
    g.drawRect(filterOnArea);
    g.drawRect(filterLPArea);
    g.drawRect(filterBPArea);
    g.drawRect(filterHPArea);
    g.drawRect(freqDialArea);
    g.drawRect(qFactorDialArea);
}

void ChannelComponent::resized()
{
    const int buttonDimension = 30; //px
    const int primaryDialDimension = 120; //px
    const int secondaryDialDimension = 80; //px

    //Buttons Bounds
    juce::Rectangle<int> channelOnArea(20, 65, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterOnArea(123, 20, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterLPArea(157, 20, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterBPArea(193, 20, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterHPArea(227, 20, buttonDimension, buttonDimension);

    //Dials Bounds
    juce::Rectangle<int> freqDialArea(109, 62, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> qFactorDialArea(189, 62, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> inGainDialArea(292, 20, primaryDialDimension, primaryDialDimension);
    juce::Rectangle<int> toneDialArea(428, 40, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> outGainDialArea(524, 20, primaryDialDimension, primaryDialDimension);
    juce::Rectangle<int> drywetDialArea(660, 40, secondaryDialDimension, secondaryDialDimension);
    
        

    //SETTING BOUNDS
    freqDial.setBounds(freqDialArea);
    qFactorDial.setBounds(qFactorDialArea);
    inGainDial.setBounds(inGainDialArea);
    toneDial.setBounds(toneDialArea);
    outGainDial.setBounds(outGainDialArea);
    drywetDial.setBounds(drywetDialArea);

    channelOn.setBounds(channelOnArea);
    filterOn.setBounds(filterOnArea);
    filterLP.setBounds(filterLPArea);
    filterBP.setBounds(filterBPArea);
    filterHP.setBounds(filterHPArea);
}
