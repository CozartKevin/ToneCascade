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
    //setPlayConfigDetails(2, 2, getSampleRate(), getBlockSize());
    // Guaranteed valid audio config
    


    juce::Logger::writeToLog("Sample rate: " + juce::String(getSampleRate()));

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

//==============================================================================
// Audio Processing
void ToneCascadeAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
 
    phase = 0.0f;
    
    bypassGain.reset(sampleRate, 0.05);  //50ms fade
    bypassGain.setTargetValue(1.0f);  // start enabled

}

void ToneCascadeAudioProcessor::releaseResources()
{
    
    writer.reset();      // Flush and close writer first
    outputStream.reset(); // Then close the stream
   
}

void ToneCascadeAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //juce::ignoreUnused(midiMessages);
    
    juce::ScopedNoDenormals noDenormals;

    //Verify buffer state
    jassert(buffer.getNumChannels() > 0);
    jassert(buffer.getNumSamples() > 0);

    // Sample-by-sample processing for smooth gain
    for (int i = 0; i < buffer.getNumSamples(); ++i) {

        const float gain = bypassGain.getNextValue(); // Smooth ramp value

        // MODIFIED: Passthrough now multiplies by gain
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            buffer.getWritePointer(ch)[i] *= gain;  // Passthrough * gain 
        }
    }


}

void ToneCascadeAudioProcessor::generateTone(juce::AudioSampleBuffer& buffer)
{
    // 1. MUST clear buffers first (prevents feedback)
    buffer.clear();

    // 2. Verified tone generation
    static float phase = 0.0f;
    const float freq = 440.0f; // A4 note
    const float phaseDelta = freq * juce::MathConstants<float>::twoPi / getSampleRate();

    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        float sample = 0.2f * std::sin(phase); // -14dB headroom
        phase += phaseDelta;

        // 3. Write to ALL channels
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            buffer.setSample(ch, i, sample);
    }

    // 4. Professional phase wrapping
    if (phase >= juce::MathConstants<float>::twoPi)
        phase -= juce::MathConstants<float>::twoPi;
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

    // Sensitivity parameter - core properties only
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "sensitivity",       // Internal ID (lowercase)
        "Sensitivity",       // Display name
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f),
        0.5f                // Default
    ));

    return layout;
}

void ToneCascadeAudioProcessor::someCommonMethod()
{
    // Add actual implementation or temporary:
    jassertfalse; // Flag for future implementation
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ToneCascadeAudioProcessor();
}
