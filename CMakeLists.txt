cmake_minimum_required(VERSION 3.22)  # JUCE requires CMake 3.15+
project(ToneCascade LANGUAGES C CXX)    # C++ project

# Find JUCE (assumes JUCE is installed or a submodule)
find_package(JUCE CONFIG REQUIRED)

# -----------------------------------------------------------------------------
# Plugin Target
# -----------------------------------------------------------------------------
juce_add_plugin(ToneCascade
    VERSION 0.1.0
    PRODUCT_NAME "ToneCascade"      # Plugin name
    COMPANY_NAME "Kevin Cozart"         # Your company/author
    IS_SYNTH FALSE                 # Not a synth
    NEEDS_MIDI_INPUT FALSE         # Disable MIDI if unused
    FORMATS VST3 Standalone        # Plugin formats (adjust as needed)
)

# -----------------------------------------------------------------------------
# Core Sources (Start Minimal)
# -----------------------------------------------------------------------------
target_sources(ToneCascade PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)

# -----------------------------------------------------------------------------
# Compiler Settings
# -----------------------------------------------------------------------------
target_compile_features(ToneCascade PRIVATE cxx_std_17)  # Require C++17
set_target_properties(ToneCascade PROPERTIES CXX_EXTENSIONS OFF)  # Disable compiler extensions (e.g., GNUisms)

# -----------------------------------------------------------------------------
# Build Instructions
# -----------------------------------------------------------------------------
# Run in terminal:
#   mkdir build && cd build
#   cmake .. -DJUCE_ROOT=/path/to/JUCE
#   cmake --build . --config Release