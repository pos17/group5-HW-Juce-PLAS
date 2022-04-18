/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ADHDAudioProcessorEditor::ADHDAudioProcessorEditor(ADHDAudioProcessor& p): AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    gainSet.setSliderStyle (juce::Slider::LinearBarVertical);
    gainSet.setRange(1.0, 20.0, 0.2);
    gainSet.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    gainSet.setPopupDisplayEnabled (true, false, this);
    gainSet.setTextValueSuffix ("Volume");
    gainSet.setValue(1.0);
    addAndMakeVisible (gainSet);
    
    gainSet.addListener (this);
}

ADHDAudioProcessorEditor::~ADHDAudioProcessorEditor()
{

}

//==============================================================================
void ADHDAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Modify Gain", getLocalBounds(), juce::Justification::centred, 1);
}

void ADHDAudioProcessorEditor::resized()
{
    gainSet.setBounds(40, 30, 20, getHeight() - 60);
}


void ADHDAudioProcessorEditor::sliderValueChanged
(juce::Slider* slider)
{audioProcessor.gain = gainSet.getValue();}
