# Include test helpers
include(${CMAKE_SOURCE_DIR}/cmake/TestHelpers.cmake)

# Unit test target
add_juce_test_target(ToneCascadeCoreTests
    SOURCES
        audio/AudioPassthroughTest.cpp
    LIBS
        ToneCascade
)

# Make sure JUCE include paths are available
target_include_directories(ToneCascadeCoreTests PRIVATE
    ${JUCE_SOURCE_DIR}/modules
)

# Add required JUCE modules
target_link_libraries(ToneCascadeCoreTests PRIVATE
    juce_core
    juce_events
    juce_unit_test
    juce_audio_basics  # Needed for audio tests
)

# Add to CTest
set_tests_properties(ToneCascadeCoreTests PROPERTIES
    LABELS "unit;core;audio"
    TIMEOUT 30
)