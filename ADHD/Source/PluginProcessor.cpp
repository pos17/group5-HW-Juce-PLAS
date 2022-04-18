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
    oversamplingModule(2, overSampFactor, juce::dsp::Oversampling<float>::FilterType::filterHalfBandFIREquiripple),
    filterL(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.0f, 0.1)),
    filterR(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.0f, 0.1))
#endif
{
    
    treeState.addParameterListener("MIDSIDE", this);
    treeState.addParameterListener("GAINL", this);
    treeState.addParameterListener("GAINR", this);
    treeState.addParameterListener("DRYWETL", this);
    treeState.addParameterListener("DRYWETR", this);
    treeState.addParameterListener("VOLUMEL", this);
    treeState.addParameterListener("VOLUMER", this);
    treeState.addParameterListener("EQONL", this);
    treeState.addParameterListener("EQSELECTL", this);
    treeState.addParameterListener("EQONR", this);
    treeState.addParameterListener("EQSELECTR", this);
    treeState.addParameterListener("QL", this);
    treeState.addParameterListener("FREQL", this);
    treeState.addParameterListener("QR", this);
    treeState.addParameterListener("FREQR", this);
}

ADHDAudioProcessor::~ADHDAudioProcessor()
{
    treeState.removeParameterListener("MIDSIDE", this);
    treeState.removeParameterListener("GAINL", this);
    treeState.removeParameterListener("GAINR", this);
    treeState.removeParameterListener("DRYWETL", this);
    treeState.removeParameterListener("DRYWETR", this);
    treeState.removeParameterListener("VOLUMEL", this);
    treeState.removeParameterListener("VOLUMER", this);
    treeState.removeParameterListener("EQONL", this);
    treeState.removeParameterListener("EQSELECTL", this);
    treeState.removeParameterListener("EQONR", this);
    treeState.removeParameterListener("EQSELECTR", this);
    treeState.removeParameterListener("QL", this);
    treeState.removeParameterListener("EQFREQL", this);
    treeState.removeParameterListener("QR", this);
    treeState.removeParameterListener("FREQR", this);
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
    lastSampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    dryBuffer = juce::AudioBuffer<float>(getTotalNumOutputChannels(), (int)(samplesPerBlock * pow(2, overSampFactor)));
    //initialization of the oversampling block specifying the maximum num of samples per block
    oversamplingModule.initProcessing(samplesPerBlock);
    filterL.prepare(spec);
    filterR.prepare(spec);
    filterL.reset();
    filterR.reset();
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

void ADHDAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    bool isInternalMidSide = isMidSide;
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    //multi channel audio block that englobes the audio buffer used as audio source before oversampling 
    juce::dsp::AudioBlock<float> srcBlock(buffer);
    juce::dsp::AudioBlock<float> overSBlock(buffer);
    juce::dsp::AudioBlock<float> oversDryBlock(dryBuffer);
    //oversampling the audio signal
    overSBlock = oversamplingModule.processSamplesUp(srcBlock);


    //Mid-Side encoding
    if (isInternalMidSide) {
        for (int sample = 0; sample < overSBlock.getNumSamples(); sample++) {
            float* dataL = overSBlock.getChannelPointer(0);
            float* dataR = overSBlock.getChannelPointer(1);

            float dataMid = dataL[sample] + dataR[sample];
            float dataSide = dataL[sample] - dataR[sample];

            dataL[sample] = dataMid * sqrt(2) / 2;
            dataR[sample] = dataSide * sqrt(2) / 2;

        }
    }
    // drycopy save before distortion
    for (int ch = 0; ch < overSBlock.getNumChannels(); ++ch) {
        float* data = overSBlock.getChannelPointer(ch);
        float* dryDataCopy = oversDryBlock.getChannelPointer(ch);

        for (int sample = 0; sample < overSBlock.getNumSamples(); sample++) {
            dryDataCopy[sample] = data[sample];

        }
    }

    //EQ BLOCK L
    if (eqOn[0]) {

        if (eqSelect[0] == 0) {
            *filterL.coefficients = *juce::dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, eqFreq[0], eqQ[0]);
            filterL.process(juce::dsp::ProcessContextReplacing <float>(overSBlock.getSingleChannelBlock(0)));
        }
        else if (eqSelect[0] == 1) {
            *filterL.coefficients = *juce::dsp::IIR::Coefficients<float>::makeBandPass(lastSampleRate, eqFreq[0], eqQ[0]);
            filterL.process(juce::dsp::ProcessContextReplacing <float>(overSBlock.getSingleChannelBlock(0)));
        }
        else if (eqSelect[0] == 2) {
            *filterL.coefficients = *juce::dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, eqFreq[0], eqQ[0]);
            filterL.process(juce::dsp::ProcessContextReplacing <float>(overSBlock.getSingleChannelBlock(0)));
        }
    }
    if (eqOn[1]) {
        if (eqSelect[1] == 0) {
            *filterL.coefficients = *juce::dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, eqFreq[1], eqQ[1]);
            filterL.process(juce::dsp::ProcessContextReplacing <float>(overSBlock.getSingleChannelBlock(1)));
        }
        else if (eqSelect[1] == 1) {
            *filterL.coefficients = *juce::dsp::IIR::Coefficients<float>::makeBandPass(lastSampleRate, eqFreq[1], eqQ[1]);
            filterL.process(juce::dsp::ProcessContextReplacing <float>(overSBlock.getSingleChannelBlock(1)));
        }
        else if (eqSelect[1] == 2) {
            *filterL.coefficients = *juce::dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, eqFreq[1], eqQ[1]);
            filterL.process(juce::dsp::ProcessContextReplacing <float>(overSBlock.getSingleChannelBlock(1)));
        }
    }

        
    
    // distortion 
    for (int ch = 0; ch < overSBlock.getNumChannels(); ++ch) {
        float* data = overSBlock.getChannelPointer(ch);
        
        for (int sample = 0; sample < overSBlock.getNumSamples(); sample++) {
            data[sample] = halfWaveAsDist(data[sample],gain[ch]);
        }
    }



    //parallel drywet channels Sum
    for (int ch = 0; ch < overSBlock.getNumChannels(); ++ch) {
        float* data = overSBlock.getChannelPointer(ch);
        float* dryDataCopy = oversDryBlock.getChannelPointer(ch);
        for (int sample = 0; sample < overSBlock.getNumSamples(); sample++) {
            
            data[sample] = (dryWet[ch] * data[sample] ) + ((1.0f - dryWet[ch]) * dryDataCopy[sample]);
            
        }
    }
    //volume application
    for (int ch = 0; ch < overSBlock.getNumChannels(); ++ch) {
        float* data = overSBlock.getChannelPointer(ch);
        float* dryDataCopy = oversDryBlock.getChannelPointer(ch);
        for (int sample = 0; sample < overSBlock.getNumSamples(); sample++) {

            data[sample] = data[sample]*volume[ch];

        }
    }
    //mid-side decoding
    if (isInternalMidSide) {
        for (int sample = 0; sample < overSBlock.getNumSamples(); sample++) {
            float* dataMid = overSBlock.getChannelPointer(0);
            float* dataSide = overSBlock.getChannelPointer(1);

            float dataL = dataMid[sample] + dataSide[sample];
            float dataR = dataMid[sample] - dataSide[sample];

            dataMid[sample] = dataL;
            dataSide[sample] = dataR;

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
    parameters.reserve(15);


    auto midSideVal = std::make_unique<juce::AudioParameterBool>("MIDSIDE", "MidSide", false);
    parameters.push_back(std::move(midSideVal));
    auto gainValL = std::make_unique<juce::AudioParameterFloat>("GAINL", "Drive Gain L/MID", 1.0f, 20.0f, 0.0f);
    parameters.push_back(std::move(gainValL));
    auto gainValR = std::make_unique<juce::AudioParameterFloat>("GAINR", "Drive Gain R/SIDE", 1.0f, 20.0f, 0.0f);
    parameters.push_back(std::move(gainValR));
    auto dryWetValL = std::make_unique<juce::AudioParameterFloat> ("DRYWETL", "Dry Wet L/MID", 0.0f, 1.0f, 1.0f);
    parameters.push_back(std::move(dryWetValL));
    auto dryWetValR = std::make_unique<juce::AudioParameterFloat>("DRYWETR", "Dry Wet R/SIDE", 0.0f, 1.0f, 1.0f);
    parameters.push_back(std::move(dryWetValR));
    auto volumeValL = std::make_unique<juce::AudioParameterFloat>("VOLUMEL", "volume L/MID", 0.0f, 1.5f, 1.0f);
    parameters.push_back(std::move(volumeValL));
    auto volumeValR = std::make_unique<juce::AudioParameterFloat>("VOLUMER", "volume R/SIDE", 0.0f, 1.5f, 1.0f);
    parameters.push_back(std::move(volumeValR));
    
    const char* eqSettingsCh[3] = { "lowPass", "bandPass","highPass"};
    const juce::StringArray eqSettings(eqSettingsCh, 3);
    auto eqOnL = std::make_unique<juce::AudioParameterBool>("EQONL", "eq On L", false);
    parameters.push_back(std::move(eqOnL));
    auto eqSelectL = std::make_unique<juce::AudioParameterChoice>("EQSELECTL","eq select L",eqSettings,0);
    parameters.push_back(std::move(eqSelectL));
    auto eqQL = std::make_unique<juce::AudioParameterFloat> ("QL", "Q factor L", 0.1f, 1.0f, 0.5f);
    parameters.push_back(std::move(eqQL));
    auto freqEqL = std::make_unique<juce::AudioParameterFloat> ("FREQL", "CutOff freq L", 20.0f, 5000.0f,100.0f);
    parameters.push_back(std::move(freqEqL));
    auto eqOnR = std::make_unique<juce::AudioParameterBool>("EQONR", "eq On R", false);
    parameters.push_back(std::move(eqOnR));
    auto eqSelectR = std::make_unique<juce::AudioParameterChoice>("EQSELECTR", "eq select R", eqSettings, 0);
    parameters.push_back(std::move(eqSelectR));
    auto eqQR = std::make_unique<juce::AudioParameterFloat>("QR", "Q factor R", 0.1f, 1.0f, 0.5f);
    parameters.push_back(std::move(eqQR));
    auto freqEqR = std::make_unique<juce::AudioParameterFloat>("FREQR", "CutOff freq R", 20.0f, 5000.0f,100.0f);
    parameters.push_back(std::move(freqEqR));
    
    return { parameters.begin(),parameters.end() };
}


void ADHDAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue) {
    if (parameterID == "GAINL") {
        gain[0] = newValue;
        
    }else if (parameterID == "GAINR") {
        gain[1] = newValue;

    } else if (parameterID == "DRYWETL") {
            dryWet[0] = newValue;
    }
    else if (parameterID == "DRYWETR") {
        dryWet[1] = newValue;
    }
    else if (parameterID == "VOLUMEL") {
        volume[0] = newValue;
    }
    else if (parameterID == "VOLUMER") {
        volume[1] = newValue;
    }
    else if (parameterID == "MIDSIDE") {
        isMidSide = (bool)newValue;
    }
    else if (parameterID == "EQONL") {
        eqOn[0] = (bool)newValue;
    }
    else if (parameterID == "EQONR") {
        eqOn[1] = (bool)newValue;
    }
    else if (parameterID == "QL") {
        eqQ[0] = newValue;
    }
    else if (parameterID == "QR") {
        eqQ[1] = newValue;
    }
    else if (parameterID == "FREQL") {
        eqFreq[0] = newValue;
    }
    else if (parameterID == "FREQR") {
        eqFreq[1] = newValue;
    }
    else if (parameterID == "EQSELECTL") {
        eqSelect[0] = newValue;
    }
    else if (parameterID == "EQSELECTR") {
        eqSelect[1] = newValue;
    }
}