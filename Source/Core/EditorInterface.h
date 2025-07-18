// Core/EditorInterface.h
#include "../JuceIncludes.h"

#pragma once
class EditorInterface {
public:
    virtual ~EditorInterface() = default;
    virtual void someCommonMethod() = 0;  // Replace with actual UI-required methods

    //Example Methods

    // Methods the UI needs(e.g., parameter access)
    virtual float getParameterValue(int paramID) const = 0;
    virtual void addParameterListener(juce::AudioProcessorParameter::Listener*) = 0;
};