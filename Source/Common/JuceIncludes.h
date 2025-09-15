#pragma once

// Core JUCE modules for audio processing and plugin development
#include <juce_core/juce_core.h>                   // Core classes like String, File, etc.
#include <juce_events/juce_events.h>               // Event handling
#include <juce_graphics/juce_graphics.h>           // Drawing primitives, Colours, Fonts
#include <juce_gui_basics/juce_gui_basics.h>       // Component, setSize(), getLookAndFeel()
#include <juce_audio_basics/juce_audio_basics.h>   // Basic audio data classes
#include <juce_audio_processors/juce_audio_processors.h> // AudioProcessor, AudioProcessorEditor, plugin host
#include <juce_audio_utils/juce_audio_utils.h>     // Utilities like audio format readers, MIDI
#include <juce_opengl/juce_opengl.h>               // OpenGL context and helpers
#include <juce_dsp/juce_dsp.h>					   // DSP classes (FFT, filters, ect)
#include <juce_audio_devices/juce_audio_devices.h> // Audio Device management
