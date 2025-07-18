/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../Core/EditorInterface.h"
#include "../Core/Parameters.h"
#include "JuceIncludes.h"


//==============================================================================
/**
*/

// Forward declaration to avoid including PluginProcessor.h
class ToneCascadeAudioProcessor;

//==============================================================================
class ToneCascadeAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    // Keep only ONE constructor declaration
    explicit ToneCascadeAudioProcessorEditor(EditorInterface& pr);
    ~ToneCascadeAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
  
     EditorInterface& processor;  // Keep this if you only need interface methods

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ToneCascadeAudioProcessorEditor)
};