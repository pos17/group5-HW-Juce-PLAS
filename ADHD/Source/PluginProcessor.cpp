/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <math.h>

//==============================================================================
ADHDAudioProcessor::ADHDAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    treeState(*this, nullptr, "Parameters", createParameters()),
    oversamplingModule(2, 2, juce::dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple)
#endif
{
    overSampFactor = 2;
    dryBuffer = juce::AudioBuffer<float>();
    treeState.addParameterListener("GAIN", this);
    treeState.addParameterListener("DRYWET", this);
}

ADHDAudioProcessor::~ADHDAudioProcessor()
{
    treeState.removeParameterListener("GAIN", this);
    treeState.removeParameterListener("DRYWET", this);
}

//==============================================================================
const juce::String ADHDAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ADHDAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ADHDAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ADHDAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ADHDAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ADHDAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ADHDAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ADHDAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ADHDAudioProcessor::getProgramName (int index)
{
    return {};
}

void ADHDAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ADHDAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    //initialization of the dsp module in order to process the audio in input to the plugin
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    dryBuffer = juce::AudioBuffer<float>(getTotalNumOutputChannels(), (int)(samplesPerBlock * pow(2, overSampFactor)));
    //initialization of the oversampling block specifying the maximum num of samples per block
    oversamplingModule.initProcessing(samplesPerBlock);

    
}

void ADHDAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ADHDAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif




bool destroy = false;

void ADHDAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //multi channel audio block that englobes the audio buffer used as audio source before oversampling 
    juce::dsp::AudioBlock<float> srcBlock (buffer);
    juce::dsp::AudioBlock<float> overSBlock (buffer);
    juce::dsp::AudioBlock<float> oversDryBlock(dryBuffer);
    //oversampling the audio signal
    overSBlock = oversamplingModule.processSamplesUp(srcBlock);

    //in this block we add all the processing to the oversampled signal
    //main processing block for distortion
    for (int ch = 0; ch < overSBlock.getNumChannels(); ++ch) {
        float* data = overSBlock.getChannelPointer(ch);
        float* dryDataCopy = oversDryBlock.getChannelPointer(ch);
        for (int sample = 0; sample < overSBlock.getNumSamples(); sample++) {
            
            
            

            dryDataCopy[sample] = data[sample];
            data[sample] = halfWaveAsDist(data[sample],gain);
        }
    }



    //parallel drywet channels Sum
    for (int ch = 0; ch < overSBlock.getNumChannels(); ++ch) {
        float* data = overSBlock.getChannelPointer(ch);
        float* dryDataCopy = oversDryBlock.getChannelPointer(ch);
        for (int sample = 0; sample < overSBlock.getNumSamples(); sample++) {
            
            data[sample] = (dryWet * data[sample]) + ((1.0f - dryWet) * dryDataCopy[sample]);
        }
    }
    oversamplingModule.processSamplesDown(srcBlock);
    
    
    
}
 float ADHDAudioProcessor::halfWaveAsDist(float sample, float gainVal) {
         if (sample > 0) {
             sample = 1 - exp(-abs(sample * gainVal));
         }
         else {
             sample = 0;
         }

     return sample;
}

//==============================================================================
bool ADHDAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ADHDAudioProcessor::createEditor()
{
    //this is commented only for the purpose of a fast parameter layout
    //for custom gui return the specific one not the generic one
 

    //return new ADHDAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void ADHDAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ADHDAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ADHDAudioProcessor();
}

//Adding the function used to create parameters in the valuetreeState
juce::AudioProcessorValueTreeState::ParameterLayout ADHDAudioProcessor::createParameters() {

    std::vector<std::unique_ptr<juce::RangedAudioParameter >> parameters;

    //reserving the space for the number of parameters we want to create
    parameters.reserve(2);

    auto gainVal = std::make_unique<juce::AudioParameterFloat>("GAIN", "Drive Gain", 1.0f, 20.0f, 0.0f);
    parameters.push_back(std::move(gainVal));
    auto dryWetVal = std::make_unique<juce::AudioParameterFloat> ("DRYWET", "Dry Wet ", 0.0f, 1.0f, 1.0f);
    parameters.push_back(std::move(dryWetVal));

        return { parameters.begin(),parameters.end() };
}


void ADHDAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue) {
    if (parameterID == "GAIN") {
        gain = newValue;
        DBG(gain);
    }else if (parameterID == "DRYWET") {
            dryWet = newValue;
    }
}