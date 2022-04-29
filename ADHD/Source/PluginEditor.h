/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ChannelComponent.h"
#include "PlasLookAndFeel.h"


//==============================================================================
/**
*/
class ADHDAudioProcessorEditor  : public juce::AudioProcessorEditor//, public juce::Slider::Listener
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

    juce::Slider destroyDial;
    juce::ToggleButton destroyButton;
    juce::ToggleButton LRButton;
    juce::ToggleButton MSButton;

    juce::ToggleButton linkButton;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADHDAudioProcessorEditor);
};
