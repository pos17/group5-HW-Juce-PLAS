/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ComboBoxTest2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ComboBoxTest2AudioProcessorEditor (ComboBoxTest2AudioProcessor&);
    ~ComboBoxTest2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ComboBoxTest2AudioProcessor& audioProcessor;
    
    juce::ComboBox choices;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> choiceAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ComboBoxTest2AudioProcessorEditor)
};
