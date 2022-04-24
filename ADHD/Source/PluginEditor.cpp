/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ADHDAudioProcessorEditor::ADHDAudioProcessorEditor (ADHDAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    // Left Channel
    /*inGainDialL.setRange(0.0, 1.0, 1. / 128);
    inGainDialL.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    inGainDialL.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    outGainDialL.setRange(0.0, 1.0, 1. / 128);
    outGainDialL.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    outGainDialL.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    clipDialL.setRange(0.0, 1.0, 1. / 128);
    clipDialL.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    clipDialL.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    toneDialL.setRange(0.0, 1.0, 1. / 128);
    toneDialL.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    toneDialL.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    freqDialL.setRange(0.0, 1.0, 1. / 128);
    freqDialL.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    freqDialL.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);


    filterOnL.setToggleState(true, false);*/

    // Right Channel
    /*inGainDialR.setRange(0.0, 1.0, 1. / 128);
    inGainDialR.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    inGainDialR.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    outGainDialR.setRange(0.0, 1.0, 1. / 128);
    outGainDialR.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    outGainDialR.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    clipDialR.setRange(0.0, 1.0, 1. / 128);
    clipDialR.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    clipDialR.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    toneDialR.setRange(0.0, 1.0, 1. / 128);
    toneDialR.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    toneDialR.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);*/


    //label2.setText("Dial2", juce::NotificationType::dontSendNotification);

    /*addAndMakeVisible(inGainDialL);
    addAndMakeVisible(outGainDialL);
    addAndMakeVisible(clipDialL);
    addAndMakeVisible(toneDialL);
    addAndMakeVisible(freqDialL);

    addAndMakeVisible(filterOnL);
    addAndMakeVisible(filterLPL);
    addAndMakeVisible(filterBPL);
    addAndMakeVisible(filterHPL);

    addAndMakeVisible(inGainDialR);
    addAndMakeVisible(outGainDialR);
    addAndMakeVisible(clipDialR);
    addAndMakeVisible(toneDialR);*/

    addAndMakeVisible(channelL);
    addAndMakeVisible(channelR);

    setSize (1000, 400);
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
    g.setFont(15.0f);
    g.drawFittedText("ADHD GUI", getLocalBounds(), juce::Justification::centredTop, 1);

    g.setColour(juce::Colours::greenyellow);
    juce::Rectangle<int> area = getLocalBounds();
    const int hArea = area.getHeight();
    const int wArea = area.getWidth();
    juce::Rectangle<int> channelNameArea(0, 0, wArea / 5, hArea);
    juce::Rectangle<int> channelAreaL(channelNameArea.getWidth(), 0, 4*wArea/5, hArea/2);
    juce::Rectangle<int> channelAreaR(channelNameArea.getWidth(), hArea/2, 4*wArea / 5, hArea / 2);

    /************LEFT CHENNEL***************/
    const int wL = channelAreaL.getWidth();
    const int hL = channelAreaL.getHeight();
    const int xL = channelAreaL.getX();
    const int yL = channelAreaL.getY();

    juce::Rectangle<int> filterContainerL(xL, yL, wL/4, hL);
    juce::Rectangle<int> dialsContainerL(xL + filterContainerL.getWidth(), yL, 3 * wL / 4, hL);

    //Filter
    const int dim = 30; //px
    const int wFL = filterContainerL.getWidth();
    const int hFL = filterContainerL.getHeight();
    const int xFL0 = filterContainerL.getX();
    const int yFL = filterContainerL.getY()+(dim/2);

    int xFL = (wFL - (4 * dim)) / 8;

    juce::Rectangle<int> filterOnArea(xFL0 + xFL, yFL, dim, dim);
    juce::Rectangle<int> filterLPArea(xFL0 + (3 * xFL) + dim, yFL, dim, dim);
    juce::Rectangle<int> filterBPArea(xFL0 + (5 * xFL) + (2*dim), yFL, dim, dim);
    juce::Rectangle<int> filterHPArea(xFL0 + (7 * xFL) + (3*dim), yFL, dim, dim);

    juce::Rectangle<int> freqDialAreaL(xFL0, yFL - (dim/2) + (hFL / 3), wFL, hFL * 2 / 3);


    // Dials
    const int wDL = dialsContainerL.getWidth();
    const int hDL = dialsContainerL.getHeight();
    const int xDL = dialsContainerL.getX();
    const int yDL = dialsContainerL.getY();

    juce::Rectangle<int> inGainDialAreaL(xDL, yDL, wDL / 3, hDL);
    juce::Rectangle<int> outGainDialAreaL(xDL+(2*wDL/3), yDL, wDL / 3, hDL);
    juce::Rectangle<int> clipDialAreaL(xDL+(wDL/3), yDL+(hDL/4), wDL/6, hDL/2);
    juce::Rectangle<int> toneDialAreaL(xDL+(wDL / 2), yDL+(hDL / 4), wDL / 6, hDL / 2);
    /**************************************/

    /************RIGHT CHENNEL***************/
    const int wR = channelAreaR.getWidth();
    const int hR = channelAreaR.getHeight();
    const int xR = channelAreaR.getX();
    const int yR = channelAreaR.getY();

    juce::Rectangle<int> filterContainerR(xR, yR, wR / 4, hR);
    juce::Rectangle<int> dialsContainerR(xR + filterContainerR.getWidth(), yR, 3 * wR / 4, hR);

    const int wDR = dialsContainerR.getWidth();
    const int hDR = dialsContainerR.getHeight();
    const int xDR = dialsContainerR.getX();
    const int yDR = dialsContainerR.getY();

    juce::Rectangle<int> inGainDialAreaR(xDR, yDR, wDR / 3, hDR);
    juce::Rectangle<int> outGainDialAreaR(xDR + (2 * wDR / 3), yDR, wDR / 3, hDR);
    juce::Rectangle<int> clipDialAreaR(xDR + (wDR / 3), yDR + (hDR / 4), wDR / 6, hDR / 2);
    juce::Rectangle<int> toneDialAreaR(xDR + (wDR / 2), yDR + (hDR / 4), wDR / 6, hDR / 2);
    /**************************************/

    g.drawRect(channelNameArea);
    g.drawRect(channelAreaL);
    g.drawRect(channelAreaR);
    g.setColour(juce::Colours::blue);
    g.drawRect(filterContainerL);
    g.drawRect(dialsContainerL);

    g.setColour(juce::Colours::white);
    g.drawRect(filterContainerR);
    g.drawRect(dialsContainerR);

    g.setColour(juce::Colours::aquamarine);
    g.drawRect(inGainDialAreaL);
    g.drawRect(outGainDialAreaL);
    g.drawRect(clipDialAreaL);
    g.drawRect(toneDialAreaL);

    g.setColour(juce::Colours::pink);
    g.drawRect(filterOnArea);
    g.drawRect(filterLPArea);
    g.drawRect(filterBPArea);
    g.drawRect(filterHPArea);
    g.drawRect(freqDialAreaL);


    g.setColour(juce::Colours::yellow);
    g.drawRect(inGainDialAreaR);
    g.drawRect(outGainDialAreaR);
    g.drawRect(clipDialAreaR);
    g.drawRect(toneDialAreaR);
}

void ADHDAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    juce::Rectangle<int> area = getLocalBounds();
    const int hArea = area.getHeight();
    const int wArea = area.getWidth();
    juce::Rectangle<int> channelNameArea(0, 0, wArea / 5, hArea);
    juce::Rectangle<int> channelAreaL(channelNameArea.getWidth(), 0, 4 * wArea / 5, hArea / 2);
    juce::Rectangle<int> channelAreaR(channelNameArea.getWidth(), hArea / 2, 4 * wArea / 5, hArea / 2);

    /************LEFT CHENNEL***************/
    /*const int wL = channelAreaL.getWidth();
    const int hL = channelAreaL.getHeight();
    const int xL = channelAreaL.getX();
    const int yL = channelAreaL.getY();

    juce::Rectangle<int> filterContainerL(xL, yL, wL / 4, hL);
    juce::Rectangle<int> dialsContainerL(xL + filterContainerL.getWidth(), yL, 3 * wL / 4, hL);*/
    
    // Input Filter
    //const int dim = 30; //px
    //const int wFL = filterContainerL.getWidth();
    //const int hFL = filterContainerL.getHeight();
    //const int xFL0 = filterContainerL.getX();
    //const int yFL = filterContainerL.getY() + (dim / 2);

    //int xFL = (wFL - (4 * dim)) / 8;

    //juce::Rectangle<int> filterOnAreaL(xFL0 + xFL, yFL, dim, dim);
    //juce::Rectangle<int> filterLPAreaL(xFL0 + (3 * xFL) + dim, yFL, dim, dim);
    //juce::Rectangle<int> filterBPAreaL(xFL0 + (5 * xFL) + (2 * dim), yFL, dim, dim);
    //juce::Rectangle<int> filterHPAreaL(xFL0 + (7 * xFL) + (3 * dim), yFL, dim, dim);

    //juce::Rectangle<int> freqDialAreaL(xFL0, yFL - (dim / 2) + (hFL / 3), wFL, hFL * 2 / 3);

    // Main Dials
    /*const int wDL = dialsContainerL.getWidth();
    const int hDL = dialsContainerL.getHeight();
    const int xDL = dialsContainerL.getX();
    const int yDL = dialsContainerL.getY();

    juce::Rectangle<int> inGainDialAreaL(xDL, yDL, wDL / 3, hDL);
    juce::Rectangle<int> outGainDialAreaL(xDL + (2 * wDL / 3), yDL, wDL / 3, hDL);
    juce::Rectangle<int> clipDialAreaL(xDL + (wDL / 3), yDL + (hDL / 4), wDL / 6, hDL / 2);
    juce::Rectangle<int> toneDialAreaL(xDL + (wDL / 2), yDL + (hDL / 4), wDL / 6, hDL / 2);*/
    /****************************************/

    /************RIGHT CHENNEL***************/
    /*const int wR = channelAreaR.getWidth();
    const int hR = channelAreaR.getHeight();
    const int xR = channelAreaR.getX();
    const int yR = channelAreaR.getY();

    juce::Rectangle<int> filterContainerR(xR, yR, wR / 4, hR);
    juce::Rectangle<int> dialsContainerR(xR + filterContainerR.getWidth(), yR, 3 * wR / 4, hR);

    const int wDR = dialsContainerR.getWidth();
    const int hDR = dialsContainerR.getHeight();
    const int xDR = dialsContainerR.getX();
    const int yDR = dialsContainerR.getY();

    juce::Rectangle<int> inGainDialAreaR(xDR, yDR, wDR / 3, hDR);
    juce::Rectangle<int> outGainDialAreaR(xDR + (2 * wDR / 3), yDR, wDR / 3, hDR);
    juce::Rectangle<int> clipDialAreaR(xDR + (wDR / 3), yDR + (hDR / 4), wDR / 6, hDR / 2);
    juce::Rectangle<int> toneDialAreaR(xDR + (wDR / 2), yDR + (hDR / 4), wDR / 6, hDR / 2);*/
    /****************************************/

    /***************BOUNDARIES***************/
    /*inGainDialL.setBounds(inGainDialAreaL);
    outGainDialL.setBounds(outGainDialAreaL);
    clipDialL.setBounds(clipDialAreaL);
    toneDialL.setBounds(toneDialAreaL);
    freqDialL.setBounds(freqDialAreaL);
    
    filterOnL.setBounds(filterOnAreaL);
    filterLPL.setBounds(filterLPAreaL);
    filterBPL.setBounds(filterBPAreaL);
    filterHPL.setBounds(filterHPAreaL);

    inGainDialR.setBounds(inGainDialAreaR);
    outGainDialR.setBounds(outGainDialAreaR);
    clipDialR.setBounds(clipDialAreaR);
    toneDialR.setBounds(toneDialAreaR);*/

    /****************************************/
    channelL.setBounds(channelAreaL);
    channelR.setBounds(channelAreaR);
}
