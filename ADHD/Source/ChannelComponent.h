/*
  ==============================================================================

    ChannelComponent.h
    Created: 19 Apr 2022 3:56:17pm
    Author:  stefa

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ChannelComponent  : public juce::Component
{
public:
    ChannelComponent();
    ~ChannelComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::Slider inGainDial;
    juce::Slider outGainDial;
    juce::Slider clipDial;
    juce::Slider toneDial;

    juce::Slider freqDial;

    juce::ToggleButton filterOn;
    juce::ToggleButton filterLP;
    juce::ToggleButton filterBP;
    juce::ToggleButton filterHP;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelComponent)
};
