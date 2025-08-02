cmake_minimum_required(VERSION 3.22)  # JUCE requires CMake 3.15+
project(ToneCascade LANGUAGES C CXX)    # C++ project

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# =============================================================================
# 1. JUCE Setup (via FetchContent)
# =============================================================================
# Find JUCE (assumes JUCE is installed or a submodule)
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.10
)
FetchContent_MakeAvailable(JUCE)

# Make JUCE modules available globally
include_directories(${JUCE_SOURCE_DIR}/modules)

# =============================================================================
# 2. Core Static Library (All testable DSP code)
# =============================================================================
add_library(ToneCascadeCore STATIC
    Source/Core/AudioEngine.cpp
    Source/Core/Parameters.h
    Source/Core/PluginProcessor.cpp
)



add_library(ToneCascadeUI STATIC
    Source/UI/CustomLook.cpp
    Source/UI/PluginEditor.cpp
    Source/UI/Visualizer.cpp
)


target_include_directories(ToneCascadeCore PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Core"
    "${JUCE_SOURCE_DIR}/modules" 
)

target_include_directories(ToneCascadeUI PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/UI"
)


# Organize files in IDEs (Visual Studio/Xcode/CLion)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})



# Core compiler settings (applied to both plugin and tests)
target_compile_definitions(ToneCascadeCore PRIVATE
    JUCE_DSP_ENABLE_SNAP_TO_ZERO=1  # Critical for denormal handling
)


# =============================================================================
# 3. JUCE Plugin Target
# =============================================================================

juce_add_plugin(ToneCascade
    VERSION 0.1.0
    PRODUCT_NAME "ToneCascade"
    COMPANY_NAME "CozartKevin"
    PLUGIN_CODE "ToCa"
    PLUGIN_MANUFACTURER_CODE "CozK"
    
    # Core configuration
    FORMATS VST3 Standalone
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    

    # All JUCE modules in REQUIRES
    REQUIRES
        juce_core
        juce_events
        juce_unit_test
        juce_audio_basics       # Core audio processing
        juce_audio_processors   # Plugin hosting
        juce_audio_utils        # Audio utilities
        juce_audio_devices      # Device I/O
        juce_audio_formats      # File I/O
        juce_dsp                # DSP algorithms
        juce_gui_basics         # UI components
        juce_graphics           # Drawing
        juce_opengl             # GL rendering
        
)



target_link_libraries(ToneCascade PRIVATE
    ToneCascadeCore
    ToneCascadeUI
    juce_audio_utils        # Audio utilities for standalone build
    juce_audio_devices      # Device I/O for standalone build
    juce_recommended_config_flags  # Compiler optimizations
    OpenMP::OpenMP_CXX                   # Parallel processing
    
)

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

 # Critical additional steps:
target_include_directories(ToneCascade PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/UI"
)

# For shared headers
target_include_directories(ToneCascade PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Core"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/UI"
    "${JUCE_SOURCE_DIR}/modules"
)

# =============================================================================
# 4. Testing (Only built if enabled)
# =============================================================================
option(BUILD_TESTING "Build unit tests" OFF)

if(BUILD_TESTING)
    # -------------------------------------------------------------------------
    # Test Executable
    # -------------------------------------------------------------------------
    add_executable(ToneCascadeTests
        Source/tests/core/audio/AudioPassthroughTest.cpp
        # Other test files...
    )



    # Link against core library + test framework
    target_link_libraries(ToneCascadeTests PRIVATE
        ToneCascadeCore  # Reusable core
        juce_unit_test   # No namespace with FetchContent
    )

    # Test-specific includes
    target_include_directories(ToneCascadeTests PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Source"

    )

    # Required for JUCE test framework
    target_compile_definitions(ToneCascadeTests PRIVATE
        JUCE_UNIT_TESTS=1
    )
   # -------------------------------------------------------------------------
    # Test Registration
    # -------------------------------------------------------------------------
    enable_testing()
    add_test(NAME ToneCascadeTests COMMAND ToneCascadeTests)
endif()



# =============================================================================
# 5. Platform Specifics
# =============================================================================
if(WIN32)
    target_link_libraries(ToneCascade PRIVATE opengl32)
    target_compile_definitions(ToneCascade PRIVATE
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_WASAPI=1
    )
elseif(APPLE)
    target_link_libraries(ToneCascade PRIVATE "-framework OpenGL")
endif()


# Platform-specific requirements
if(JucePlugin_Build_Standalone)
    # Additional standalone-only dependencies
    target_compile_definitions(ToneCascade PRIVATE
        JUCE_STANDALONE_APPLICATION=1
    )

   
target_link_libraries(ToneCascade PRIVATE
    juce_audio_utils        # Audio utilities for standalone build
    juce_audio_devices      # Device I/O for standalone build
)

endif()

# Parallel processing
find_package(OpenMP REQUIRED)  # Force OpenMP for your FFT
target_link_libraries(ToneCascade PRIVATE OpenMP::OpenMP_CXX)


# =============================================================================
# 6. IDE Organization
# =============================================================================
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Source" PREFIX "CORE"
    FILES
        Source/Core/*.cpp
        Source/Core/*.h
 )

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Source" PREFIX "UI"
    FILES   
        Source/UI/*.cpp
        Source/UI/*.h
)

if(BUILD_TESTING)
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Source/Tests" PREFIX "CoreAudioTests"
        FILES
            Source/Tests/core/audio/*.cpp
    )
   
endif()




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

if(MSVC)
    # Critical MSVC settings for JUCE projects
    add_compile_options(
        /bigobj          # Required for JUCE templates
        /Zm1500          # 1.5GB heap (adjust between 1000-2000 as needed)
        /MP2             # Moderate parallelism
        /EHsc            # Exception handling
    )
    
    # Reduce memory pressure
    set(CMAKE_VS_GLOBALS
        "UseMultiToolTask=true"
        "EnforceProcessCountAcrossBuilds=true"
        "MaxToolTasks=2"
    )
endif()

target_compile_features(ToneCascade PRIVATE cxx_std_17)  # Require C++17
set_target_properties(ToneCascade PROPERTIES CXX_EXTENSIONS OFF)  # Disable compiler extensions (e.g., GNUisms)

# -----------------------------------------------------------------------------
# Build Instructions
# -----------------------------------------------------------------------------
# Run in terminal:
#   mkdir build && cd build
#   cmake .. -G "Visual Studio 17 2022" -A x64
#   cmake --build . --config Release