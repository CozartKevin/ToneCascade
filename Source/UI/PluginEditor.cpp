/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/


#include "PluginEditor.h"

//==============================================================================
ToneCascadeAudioProcessorEditor::ToneCascadeAudioProcessorEditor(EditorInterface& p)
    : juce::AudioProcessorEditor(dynamic_cast<juce::AudioProcessor&>(p)),
    processor(p)
{
    setSize(400, 300);
}

ToneCascadeAudioProcessorEditor::~ToneCascadeAudioProcessorEditor()
{
}

void ToneCascadeAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void ToneCascadeAudioProcessorEditor::resized()
{
    // Component layout
}