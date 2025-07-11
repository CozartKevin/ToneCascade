/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once


#include "../Core/Parameters.h"
#include "../Core/EditorInterface.h"
#include "JuceIncludes.h"

//==============================================================================
/**
*/
class ToneCascadeAudioProcessor : public juce::AudioProcessor,
                                 public EditorInterface
{
public:
    //========================================================================
    // Core Audio Processor Functions
    ToneCascadeAudioProcessor();
    ~ToneCascadeAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //========================================================================
    // EditorInterface Implementation
    float getParameterValue(int paramID) const override;
    void addParameterListener(juce::AudioProcessorParameter::Listener*) override;

    //========================================================================
    // JUCE Standard Overrides
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    
    #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    #endif

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();


private:
    float lastSampleRate = 0.0f;
    int lastBlockSize = 0;
    juce::AudioProcessorValueTreeState apvts;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToneCascadeAudioProcessor)
};