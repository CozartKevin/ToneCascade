/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/


#include "PluginEditor.h"

//==============================================================================
ToneCascadeAudioProcessorEditor::ToneCascadeAudioProcessorEditor(EditorInterface& proc)
    : juce::AudioProcessorEditor(dynamic_cast<juce::AudioProcessor&>(proc)),
    processor(proc)
{
    setSize(400, 300);
}

ToneCascadeAudioProcessorEditor::~ToneCascadeAudioProcessorEditor()
{
}

//==============================================================================
void ToneCascadeAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::Font(15.0f));  

    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void ToneCascadeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

explicit ToneCascadeAudioProcessorEditor(ToneCascadeAudioProcessor& p)
    : juce::AudioProcessorEditor(&p),  // Initialize base class
    audioProcessor(p)                // Initialize reference
{
}