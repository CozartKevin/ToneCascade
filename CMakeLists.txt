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

    # Core Audio/DSP
    Source/Core/AudioEngine.cpp
    Source/Core/AudioEngine.h
    Source/Core/Parameters.h
    Source/Core/PluginProcessor.cpp
    Source/Core/PluginProcessor.h

    # Standalone Entry Point (Optional)
    Source/Core/Standalone/Main.cpp

    # UI
    Source/UI/CustomLook.cpp
    Source/UI/CustomLook.h
    Source/UI/PluginEditor.cpp
    Source/UI/PluginEditor.h
    Source/UI/Visualizer.cpp
    Source/UI/Visualizer.h
   
)

# Optional but helpful for IDE organization
source_group("Core" FILES
    Source/Core/*.cpp
    Source/Core/*.h
)
source_group("UI" FILES
    Source/UI/*.cpp
    Source/UI/*.h
)

# Critical additional steps:
target_include_directories(ToneCascade PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/UI"
)

# For shared headers
target_include_directories(ToneCascade PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
)

# Organize files in IDEs (Visual Studio/Xcode/CLion)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

# -----------------------------------------------------------------------------
# Compiler Settings
# -----------------------------------------------------------------------------
target_compile_definitions(ToneCascade PRIVATE 
    JUCE_MODAL_LOOPS_PERMITTED=0  # Disable GUI features in Core
)
# Fail if UI tries to include Core headers indirectly
target_compile_definitions(ToneCascade PRIVATE
    JUCE_STRICT_REFCOUNTEDPTR=1
)

target_compile_features(ToneCascade PRIVATE cxx_std_17)  # Require C++17
set_target_properties(ToneCascade PROPERTIES CXX_EXTENSIONS OFF)  # Disable compiler extensions (e.g., GNUisms)

# -----------------------------------------------------------------------------
# Build Instructions
# -----------------------------------------------------------------------------
# Run in terminal:
#   mkdir build && cd build
#   cmake .. -G "Visual Studio 17 2022" -A x64
#   cmake --build . --config Release