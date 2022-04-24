/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ChannelComponent.h"

//==============================================================================
/**
*/
class ADHDAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ADHDAudioProcessorEditor (ADHDAudioProcessor&);
    ~ADHDAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ADHDAudioProcessor& audioProcessor;

    ChannelComponent channelL;
    ChannelComponent channelR;


    /*juce::Slider inGainDialL;
    juce::Slider outGainDialL;
    juce::Slider clipDialL;
    juce::Slider toneDialL;
    juce::Slider freqDialL;

    juce::ToggleButton filterOnL;
    juce::ToggleButton filterLPL;
    juce::ToggleButton filterBPL;
    juce::ToggleButton filterHPL;

    juce::Slider inGainDialR;
    juce::Slider outGainDialR;
    juce::Slider clipDialR;
    juce::Slider toneDialR;*/

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADHDAudioProcessorEditor);
};
