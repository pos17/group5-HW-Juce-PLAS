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
    //juce::ToggleButton LRButton;
    //juce::ToggleButton MSButton;
    juce::ToggleButton msLr;
    
    juce::ToggleButton linkButton;
    
    //ATTACHMENTS
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> msAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryWetRAttachment;
    
    
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freqRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qRAttachment;
    
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqOnLAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqLPLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqBPLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqHPLAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterListLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterListRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqOnRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqLPRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqBPRAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqHPRAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> channelOnLAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> channelOnRAttachment;

    
    
    
    //LOOK AND FEEL
    
    PLASBigKnobLookFeel bkLeF {};
    PLASLittleKnobLookFeel lkLeF {};
    PLASDestroyButtonLookAndFeel dbLeF {};
    PLASButLinkLookAndFeel blLeF {};
    PLASButOnLookAndFeel bOLeF {};
    PLASButLPLookAndFeel bLPLeF {};
    PLASButBPLookAndFeel bBPLeF {};
    PLASButHPLookAndFeel bHPLeF {};
    PLASMSLRLookAndFeel bMSLRLeF {};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADHDAudioProcessorEditor);
};
