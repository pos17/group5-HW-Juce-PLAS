/*
  =============================================================================

    This file contains the basic framework code for a JUCE plugin editor

  =============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//=============================================================================
ADHDAudioProcessorEditor::ADHDAudioProcessorEditor(ADHDAudioProcessor& p): AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set th
    // editor's size to whatever you need it to be
   addAndMakeVisible(channelL);
   addAndMakeVisible(channelR);

   destroyDial.setRange(0.0, 1.0, 1. / 128);
    destroyDial.setSliderStyle (juce::Slider::SliderStyle::Rotary);
    destroyDial.setTextBoxStyle (juce::Slider::NoTextBox, false,90, 0);

   LRButton.setToggleState (true, false);

    addAndMakeVisible(destroyDial);
   addAndMakeVisible(destroyButton);
   addAndMakeVisible(LRButton);
    addAndMakeVisible(MSButton);

    setSize (1000, 400);
}

ADHDAudioProcessorEditor::~ADHDAudioProcessorEditor()
{
}

//=============================================================================
void ADHDAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour
    juce::Rectangle<int> channelAreaL(30, 30, 780, 160);
    juce::Rectangle<int> channelAreaR(30, 210, 780, 160);

    const int buttonDimension = 30; //px
    const int primaryDialDimension = 120; //px
    const int secondaryDialDimension = 80; //px

    int yOffset = 30;
    int xOffset = 30;

    //Buttons Brouns
    juce::Rectangle<int> channelOnArea(20+xOffset, 65 + yOffset, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterOnArea(113 + xOffset, 20 + yOffset, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterLPArea(147 + xOffset, 20 + yOffset, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterBPArea(183 + xOffset, 20 + yOffset, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterHPArea(217 + xOffset, 20 + yOffset, buttonDimension, buttonDimension);

    //Dials Bounds
    juce::Rectangle<int> freqDialArea(141 + xOffset, 59 + yOffset, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> inGainDialArea(267 + xOffset, 17 + yOffset, primaryDialDimension, primaryDialDimension);
    juce::Rectangle<int> clipDialArea(388 + xOffset, 37 + yOffset, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> toneDialArea(469 + xOffset, 37 + yOffset, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> outGainDialArea(550 + xOffset, 17 + yOffset, primaryDialDimension, primaryDialDimension);
    juce::Rectangle<int> drywetDialArea(680 + xOffset, 37 + yOffset, secondaryDialDimension, secondaryDialDimension);

    juce::Rectangle<int> destroySectionArea(830, 30, 140, 340);

    xOffset = destroySectionArea.getX();
    yOffset = destroySectionArea.getY();

    juce::Rectangle<int> destroyButtonArea(20 + xOffset, 270 + yOffset, 100, 60);
    juce::Rectangle<int> destroyDialArea(30 + xOffset, 180 + yOffset, 80, 80);

    juce::Rectangle<int> LRButtonArea(30 + xOffset, 20 + yOffset, 30, 30);
    juce::Rectangle<int> MSButtonArea(78 + xOffset, 20 + yOffset, 30, 30);

    juce::Rectangle<int> inMeterArea(20 + xOffset, 60 + yOffset, 40, 120);
    juce::Rectangle<int> outMeterArea(80 + xOffset, 60 + yOffset, 40, 120);
    /**************************************/
    g.setColour(juce::Colours::red);
    g.drawRect(channelAreaL);
    g.drawRect(channelAreaR);
    g.drawRect(destroySectionArea);


    

    g.setColour( juce::Colours::beige);
    g.drawRect(destroyButtonArea);
    g.drawRect(destroyDialArea);
    g.drawRect(LRButtonArea);
    g.drawRect(MSButtonArea);
    g.drawRect(inMeterArea);
    g.drawRect(outMeterArea);

}

void ADHDAudioProcessorEditor::resized()
{
   // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // Channels Bounds
    juce::Rectangle<int> channelAreaL(30, 30, 780, 160);
    juce::Rectangle<int> channelAreaR(30, 210, 780, 160);

    // Destroy Bounds
    juce::Rectangle<int> destroySectionArea(830, 30, 140, 340);
    
    const int xOffset = destroySectionArea.getX();
    const int yOffset = destroySectionArea.getY();

    juce::Rectangle<int> destroyButtonArea(20 + xOffset, 270 + yOffset, 100, 60);
    juce::Rectangle<int> destroyDialArea(30 + xOffset, 180 + yOffset, 80, 80);

    juce::Rectangle<int> LRButtonArea(30 + xOffset, 20 + yOffset, 30, 30);
    juce::Rectangle<int> MSButtonArea(78 + xOffset, 20 + yOffset, 30, 30);
    
    // nSeting Bounds
    channelL.setBounds(channelAreaL);
    channelR.setBounds(channelAreaR);

    destroyButton.setBounds(destroyButtonArea);
    destroyDial.setBounds(destroyDialArea);
    LRButton.setBounds(LRButtonArea);
    MSButton.setBounds(MSButtonArea);
}

