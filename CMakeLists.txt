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
    PRODUCT_NAME "ToneCascade"
    COMPANY_NAME "CozartKevin"
    PLUGIN_CODE "ToCa"
    PLUGIN_MANUFACTURER_CODE "CozK"
    
    # Core configuration
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    FORMATS VST3 Standalone

    # All JUCE modules in REQUIRES
    REQUIRES
        juce_audio_basics       # Core audio processing
        juce_audio_processors   # Plugin hosting

        juce_audio_formats      # File I/O
        juce_dsp                # DSP algorithms
        juce_gui_basics         # UI components
        juce_graphics           # Drawing
        juce_opengl             # GL rendering
)

# Non-JUCE dependencies only
target_link_libraries(ToneCascade PRIVATE
    juce_audio_utils        # Audio utilities
    juce_audio_devices      # Device I/O
    juce::juce_recommended_config_flags  # Compiler optimizations
    OpenMP::OpenMP_CXX                   # Parallel processing
)

# Platform-specific OpenGL linking
if(APPLE)
    target_link_libraries(ToneCascade PRIVATE "-framework OpenGL")
elseif(WIN32)
    target_link_libraries(ToneCascade PRIVATE opengl32)
endif()

# Platform-specific requirements
if(JucePlugin_Build_Standalone)
    # Additional standalone-only dependencies
    target_compile_definitions(ToneCascade PRIVATE
        JUCE_STANDALONE_APPLICATION=1
    )
endif()

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

# Fail if UI tries to include Core headers indirectly
target_compile_definitions(ToneCascade PRIVATE
    JUCE_MODAL_LOOPS_PERMITTED=0
    JUCE_STRICT_REFCOUNTEDPTR=1
    JUCE_DSP_ENABLE_SNAP_TO_ZERO=1  # ← Critical for denormal handling
    JUCE_USE_VDSP_FRAMEWORK=0       # ← Avoid macOS DSP quirks
    JUCE_WASAPI=1

 

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