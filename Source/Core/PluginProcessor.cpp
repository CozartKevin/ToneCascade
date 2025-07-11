#include "PluginProcessor.h"
#include "../UI/PluginEditor.h"  // UI include ONLY in .cpp

//==============================================================================
ToneCascadeAudioProcessor::ToneCascadeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
#endif
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    // Parameter initialization
}

ToneCascadeAudioProcessor::~ToneCascadeAudioProcessor()
{
}

//==============================================================================
// EditorInterface Implementation
float ToneCascadeAudioProcessor::getParameterValue(int paramID) const
{
    auto* param = apvts.getParameter("param" + std::to_string(paramID));
    return param ? param->getValue() : 0.0f;
}

void ToneCascadeAudioProcessor::addParameterListener(
    juce::AudioProcessorParameter::Listener* listener)
{
    for (auto* param : getParameters()) {
        param->addListener(listener);
    }
}

void ToneCascadeAudioProcessor::someCommonMethod()
{
    // Your implementation
}

//==============================================================================
// Audio Processing
void ToneCascadeAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    lastSampleRate = sampleRate;
    lastBlockSize = samplesPerBlock;
}

void ToneCascadeAudioProcessor::releaseResources()
{
}

void ToneCascadeAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    const int numInputs = getTotalNumInputChannels();

    // Clears unused output channels
    for (int ch = numInputs; ch < numChannels; ++ch) {
        buffer.clear(ch, 0, numSamples);
    }

    // DOESN'T process or pass through input channels
    for (int ch = 0; ch < numInputs; ++ch) {
        auto* data = buffer.getWritePointer(ch); // Gets pointer but does nothing
    }
}

//==============================================================================
// JUCE Standard Implementations
juce::AudioProcessorEditor* ToneCascadeAudioProcessor::createEditor()
{
    return new ToneCascadeAudioProcessorEditor(*this);
}

bool ToneCascadeAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return false;
}

const juce::String ToneCascadeAudioProcessor::getName() const
{
    return juce::String();
}

bool ToneCascadeAudioProcessor::acceptsMidi() const
{
    return false;
}

bool ToneCascadeAudioProcessor::producesMidi() const
{
    return false;
}

bool ToneCascadeAudioProcessor::isMidiEffect() const
{
    return false;
}

double ToneCascadeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ToneCascadeAudioProcessor::getNumPrograms()
{
    return 0;
}

int ToneCascadeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ToneCascadeAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String ToneCascadeAudioProcessor::getProgramName(int index)
{
    return juce::String();
}

void ToneCascadeAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void ToneCascadeAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void ToneCascadeAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

// ... (keep other standard JUCE method implementations as-is) ...

//==============================================================================
// Parameter Layout
juce::AudioProcessorValueTreeState::ParameterLayout
ToneCascadeAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Add your parameters here
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "gain",
        "Gain",
        juce::NormalisableRange<float>(0.0f, 1.0f),
        0.5f));

    return layout;
}

void ToneCascadeAudioProcessor::someCommonMethod()
{
    // Add actual implementation or temporary:
    jassertfalse; // Flag for future implementation
}