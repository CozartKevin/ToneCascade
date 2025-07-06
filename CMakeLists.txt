cmake_minimum_required(VERSION 3.22)  # JUCE requires CMake 3.15+
project(ToneCascade LANGUAGES C CXX)    # C++ project

if(WIN32)
  
    add_compile_definitions(
        # Nuclear options for VST2
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_VST3_EMULATE_VST2=0
        JUCE_VST3_EMULATE_VST2_MODE=0
        VST2_CAN_REPLACE_VST3=0    
    )
    

endif()

# Find JUCE (assumes JUCE is installed or a submodule)
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.10
)
FetchContent_MakeAvailable(JUCE)

# -----------------------------------------------------------------------------
# Plugin Target
# -----------------------------------------------------------------------------
juce_add_plugin(ToneCascade
    VERSION 0.1.0
    PRODUCT_NAME "ToneCascade"      # Plugin name
    COMPANY_NAME "KevinCozart"         # Your company/author
    IS_SYNTH FALSE                 # Not a synth
    NEEDS_MIDI_INPUT FALSE         # Disable MIDI if unused
    FORMATS VST3 Standalone         # Plugin formats (adjust as needed)
    PLUGIN_CODE "ToCa"
    PLUGIN_MANUFACTURER_CODE KevC
    COPY_PLUGIN_AFTER_BUILD FALSE
)


# Core JUCE + OpenGL
target_link_libraries(ToneCascade PRIVATE
    juce::juce_recommended_config_flags  # Best practice compiler flags
    juce::juce_audio_utils
    juce::juce_audio_devices
    juce::juce_gui_basics
    juce::juce_graphics
    juce::juce_opengl
    juce::juce_dsp
)

# Parallel processing
find_package(OpenMP REQUIRED)  # Force OpenMP for your FFT
target_link_libraries(ToneCascade PRIVATE OpenMP::OpenMP_CXX)



# -----------------------------------------------------------------------------
# Core Sources (Start Minimal)
# -----------------------------------------------------------------------------
target_sources(ToneCascade PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/JuceIncludes.h
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
#   cmake .. -G "Visual Studio 17 2022" -A x64
#   cmake --build . --config Release