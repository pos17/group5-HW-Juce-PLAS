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
    // Dials
    juce::Slider freqDial;
    juce::Slider qFactorDial;

    juce::Slider inGainDial;
    juce::Slider outGainDial;
    juce::Slider toneDial;

    juce::Slider drywetDial;    


    // Buttons
    juce::ToggleButton channelOn;

    juce::ToggleButton filterOn;
    juce::ToggleButton filterLP;
    juce::ToggleButton filterBP;
    juce::ToggleButton filterHP;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelComponent)
};
