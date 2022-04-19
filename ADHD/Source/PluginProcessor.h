/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class ADHDAudioProcessor : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    ADHDAudioProcessor();
    ~ADHDAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    


    juce::AudioProcessorValueTreeState treeState;

    //declaration of variables
    
    
    
private:
    bool bypass{ false };
    bool isMidSide{ false };
    float gain[2]{ 1.0,1.0 };
    float dryWet[2]{1.0,1.0};
    float volume[2]{ 1.0,1.0 };
    bool eqOn[2]{ false,false };
    int eqSelect[2]{ 0,0 };
    float eqQ[2]{ 0.1f,0.1f };
    float eqFreq[2]{ 100.0f,100.0f };
    int overSampFactor{2};
    int lastSampleRate{44100};
    juce::AudioBuffer<float> dryBuffer = juce::AudioBuffer<float>();
    
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    //custom private functions 
        
    float halfWaveAsDist(float sample,float gainVal);
    
    //dsp modules declaration

    juce::dsp::Oversampling<float> oversamplingModule;
    juce::dsp::IIR::Filter<float> filterL;
    juce::dsp::IIR::Filter<float> filterR;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADHDAudioProcessor)
    

};
