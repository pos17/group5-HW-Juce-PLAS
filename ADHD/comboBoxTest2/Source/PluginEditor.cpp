/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ComboBoxTest2AudioProcessorEditor::ComboBoxTest2AudioProcessorEditor (ComboBoxTest2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    choiceAttachment.reset (new juce::AudioProcessorValueTreeState::ComboBoxAttachment (audioProcessor.treeState, "CHOICE", choices));
    choices.addItem("choice1", 1);
    choices.addItem("choice2", 2);
    choices.addItem("choice3", 3);
    addAndMakeVisible(choices);
    setSize (400, 300);
}

ComboBoxTest2AudioProcessorEditor::~ComboBoxTest2AudioProcessorEditor()
{
}

//==============================================================================
void ComboBoxTest2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    juce::String ciaoStr = std::to_string(audioProcessor.ciao);
    g.drawFittedText (ciaoStr, getLocalBounds(), juce::Justification::centred, 1);
}

void ComboBoxTest2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    choices.setBounds(10, 10, 80, 30);
}
