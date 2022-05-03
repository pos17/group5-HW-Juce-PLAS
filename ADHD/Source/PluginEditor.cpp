/*
  =============================================================================

    This file contains the basic framework code for a JUCE plugin editor

  =============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ChannelComponent.h"

//=============================================================================
ADHDAudioProcessorEditor::ADHDAudioProcessorEditor(ADHDAudioProcessor& p): AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    destroyDial.setRange(0.0, 1.0, 1. / 128);
    destroyDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    destroyDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    destroyDial.setLookAndFeel(&lkLeF);
    //LRButton.setToggleState(true, false);
    //linkButton.setToggleState(true, false);
    
    
    linkButton.onClick = [this] {
        bool stateLink = linkButton.getToggleState();
        if(stateLink)
            setLinkedAttachments();
        else {
            setUnlinkedAttachments();
        }
    };
    //ATTACHMENTS
    linkAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "LINK", linkButton));
    msAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "MIDSIDE", msLr));
    destroyAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "DESTROY", destroyButton));
    gainLLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "GAINL", *channelL.getInGainDial()));
    gainRRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "GAINR", *channelR.getInGainDial()));
    volumeLLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "VOLUMEL", *channelL.getOutGainDial()));
    volumeRRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "VOLUMER", *channelR.getOutGainDial()));
    dryWetLLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "DRYWETL", *channelL.getDryWetDial()));
    dryWetRRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "DRYWETR", *channelR.getDryWetDial()));
    freqLLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "FREQL", *channelL.getFreqDial()));
    freqRRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "FREQR", *channelR.getFreqDial()));
    qLLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "QL", *channelL.getQFactorDial()));
    qRRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "QR", *channelR.getQFactorDial()));
    
    eqLPLLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQLPL", *channelL.getFilterLP()));
    eqBPLLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQBPL", *channelL.getFilterBP()));
    eqHPLLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQHPL", *channelL.getFilterHP()));
     
    filterListLAttachment.reset (new juce::AudioProcessorValueTreeState::ComboBoxAttachment (audioProcessor.treeState, "EQSELECTL", *channelL.getFilterList()));
    filterListRAttachment.reset (new juce::AudioProcessorValueTreeState::ComboBoxAttachment (audioProcessor.treeState, "EQSELECTR", *channelR.getFilterList()));
    
    eqLPRRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQLPR", *channelR.getFilterLP()));
    eqBPRRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQBPR", *channelR.getFilterBP()));
    eqHPRRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQHPR", *channelR.getFilterHP()));
    
    eqOnLLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQONL", *channelL.getFilterOn()));
    eqOnRRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQONR", *channelR.getFilterOn()));
    channelOnLLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "CHANNELONL", *channelL.getChannelOn()));
    channelOnRRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "CHANNELONR", *channelR.getChannelOn()));


    destroyButton.setLookAndFeel(&dbLeF);
    
    linkButton.setLookAndFeel(&blLeF);
    //MSButton.setLookAndFeel(&bMSLeF);
    //LRButton.setLookAndFeel(&bLRLeF);
    msLr.setLookAndFeel(&bMSLRLeF);

    //ADD AND MAKE VISIBLE
    addAndMakeVisible(channelL);
    addAndMakeVisible(channelR);
    channelL.setBKLeF(&bkLeF);
    channelR.setBKLeF(&bkLeF);
    channelL.setLKLeF(&lkLeF);
    channelR.setLKLeF(&lkLeF);
    channelL.setOnButtonsLeF(&bOLeF);
    channelR.setOnButtonsLeF(&bOLeF);
    channelL.setLPButtonsLeF(&bLPLeF);
    channelR.setLPButtonsLeF(&bLPLeF);
    channelL.setBPButtonsLeF(&bBPLeF);
    channelR.setBPButtonsLeF(&bBPLeF);
    channelL.setHPButtonsLeF(&bHPLeF);
    channelR.setHPButtonsLeF(&bHPLeF);
    
    addAndMakeVisible(destroyDial);
    addAndMakeVisible(destroyButton);
    //addAndMakeVisible(LRButton);
    //addAndMakeVisible(MSButton);
    addAndMakeVisible(msLr);
    
    addAndMakeVisible(linkButton);

    //LISTENERS
    //destroyDial.addListener(this);
    destroyButton.onClick = [this] {
        bool state = destroyButton.getToggleState();
        bkLeF.setDestroy(state);
        lkLeF.setDestroy(state);
        blLeF.setDestroy(state);
        bOLeF.setDestroy(state);
        bLPLeF.setDestroy(state);
        bBPLeF.setDestroy(state);
        bHPLeF.setDestroy(state);
        bMSLRLeF.setDestroy(state);
        setVisible(false);
        setVisible(true);
    };
    
    destroyButton.onClick = [this] {
        bool state = destroyButton.getToggleState();
        bkLeF.setDestroy(state);
        lkLeF.setDestroy(state);
        blLeF.setDestroy(state);
        bOLeF.setDestroy(state);
        bLPLeF.setDestroy(state);
        bBPLeF.setDestroy(state);
        bHPLeF.setDestroy(state);
        bMSLRLeF.setDestroy(state);
        setVisible(false);
        setVisible(true);
    };

    bool state = destroyButton.getToggleState();
    bkLeF.setDestroy(state);
    lkLeF.setDestroy(state);
    blLeF.setDestroy(state);
    bOLeF.setDestroy(state);
    bLPLeF.setDestroy(state);
    bBPLeF.setDestroy(state);
    bHPLeF.setDestroy(state);
    bMSLRLeF.setDestroy(state);
    setVisible(false);
    setVisible(true);

    setSize (1000, 400);
}

ADHDAudioProcessorEditor::~ADHDAudioProcessorEditor()
{
    channelL.setNullLookAndFeel();
    channelR.setNullLookAndFeel();

    destroyButton.setLookAndFeel(nullptr);
    destroyDial.setLookAndFeel(nullptr);
    msLr.setLookAndFeel(nullptr);
    linkButton.setLookAndFeel(nullptr);
}

//=============================================================================
void ADHDAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.drawImageAt(juce::ImageCache::getFromMemory(BinaryData::GUI_definitivaBGMS_png, BinaryData::GUI_definitivaBGMS_pngSize), 0, 0);


    // (Our component is opaque, so we must completely fill the background with a solid colour)
    juce::Rectangle<int> channelAreaL(30, 30, 780, 160);
    juce::Rectangle<int> channelAreaR(30, 210, 780, 160);

    const int buttonDimension = 30; //px
    const int primaryDialDimension = 120; //px
    const int secondaryDialDimension = 80; //px

    int yOffset = 30;
    int xOffset = 30;

    //Buttons Bounds
    juce::Rectangle<int> linkButtonArea(109, 181, 22, 38);

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

    //juce::Rectangle<int> LRButtonArea(30 + xOffset, 20 + yOffset, 30, 30);
    //juce::Rectangle<int> MSButtonArea(78 + xOffset, 20 + yOffset, 30, 30);
    juce::Rectangle<int> msLrArea(30 + xOffset, 20 + yOffset, 78, 30);
    
    
    juce::Rectangle<int> inMeterArea(22 + xOffset, 60 + yOffset, 38, 114);
    juce::Rectangle<int> outMeterArea(80 + xOffset, 60 + yOffset, 38, 114);
    /**************************************/
    /*g.setColour(juce::Colours::red);
    g.drawRect(channelAreaL);
    g.drawRect(channelAreaR);
    g.drawRect(destroySectionArea);
    g.drawRect(linkButtonArea);

    g.setColour( juce::Colours::beige);
    g.drawRect(destroyButtonArea);
    g.drawRect(destroyDialArea);*/
    
    
    //g.drawRect(LRButtonArea);
    //g.drawRect(MSButtonArea);
    //g.drawRect(msLrArea);
    
    
    g.drawRect(inMeterArea);
    g.drawRect(outMeterArea);

}

void ADHDAudioProcessorEditor::resized()
{
   // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // Link Button Bounds
    juce::Rectangle<int> linkButtonArea(109, 181, 22, 38);

    // Channels Bounds
    juce::Rectangle<int> channelAreaL(30, 30, 780, 160);
    juce::Rectangle<int> channelAreaR(30, 210, 780, 160);

    // Destroy Bounds
    juce::Rectangle<int> destroySectionArea(830, 30, 140, 340);
    
    const int xOffset = destroySectionArea.getX();
    const int yOffset = destroySectionArea.getY();

    juce::Rectangle<int> destroyButtonArea(20 + xOffset, 270 + yOffset, 100, 60);
    juce::Rectangle<int> destroyDialArea(30 + xOffset, 180 + yOffset, 80, 80);

    //juce::Rectangle<int> LRButtonArea(30 + xOffset, 20 + yOffset, 30, 30);
    //juce::Rectangle<int> MSButtonArea(78 + xOffset, 20 + yOffset, 30, 30);
    juce::Rectangle<int> msLrArea(30 + xOffset, 20 + yOffset, 78, 30);
    // Setting Bounds
    linkButton.setBounds(linkButtonArea);

    channelL.setBounds(channelAreaL);
    channelR.setBounds(channelAreaR);

    destroyButton.setBounds(destroyButtonArea);
    destroyDial.setBounds(destroyDialArea);
    //LRButton.setBounds(LRButtonArea);
    //MSButton.setBounds(MSButtonArea);
    msLr.setBounds(msLrArea);
}


void ADHDAudioProcessorEditor::setLinkedAttachments() {
    gainLRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "GAINR", *channelL.getInGainDial()));
    gainRLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "GAINL", *channelR.getInGainDial()));
    
    volumeLRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "VOLUMER", *channelL.getOutGainDial()));
    volumeRLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "VOLUMEL", *channelR.getOutGainDial()));
    dryWetLRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "DRYWETR", *channelL.getDryWetDial()));
    dryWetRLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "DRYWETL", *channelR.getDryWetDial()));
    freqLRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "FREQR", *channelL.getFreqDial()));
    freqRLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "FREQL", *channelR.getFreqDial()));
    qLRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "QR", *channelL.getQFactorDial()));
    qRLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "QL", *channelR.getQFactorDial()));
    
    eqLPLRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQLPR", *channelL.getFilterLP()));
    eqBPLRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQBPR", *channelL.getFilterBP()));
    eqHPLRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQHPR", *channelL.getFilterHP()));
    
    eqLPRLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQLPL", *channelR.getFilterLP()));
    eqBPRLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQBPL", *channelR.getFilterBP()));
    eqHPRLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQHPL", *channelR.getFilterHP()));
    
    eqOnLRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQONR", *channelL.getFilterOn()));
    eqOnRLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQONL", *channelR.getFilterOn()));
    channelOnLRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "CHANNELONR", *channelL.getChannelOn()));
    channelOnRLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "CHANNELONL", *channelR.getChannelOn()));
     
}

void ADHDAudioProcessorEditor::setUnlinkedAttachments() {
    gainLRAttachment.reset ();
    gainRLAttachment.reset ();
    
    volumeLRAttachment.reset ();
    volumeRLAttachment.reset ();
    dryWetLRAttachment.reset ();
    dryWetRLAttachment.reset ();
    freqLRAttachment.reset ();
    freqRLAttachment.reset ();
    qLRAttachment.reset ();
    qRLAttachment.reset ();
    
    eqLPLRAttachment.reset ();
    eqBPLRAttachment.reset ();
    eqHPLRAttachment.reset ();
    
    eqLPRLAttachment.reset ();
    eqBPRLAttachment.reset ();
    eqHPRLAttachment.reset ();
    
    eqOnLRAttachment.reset ();
    eqOnRLAttachment.reset ();
    channelOnLRAttachment.reset ();
    channelOnRLAttachment.reset ();
}
