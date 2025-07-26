# Professional test suite organization

# Set include paths for all tests
include_directories(
    ${JUCE_SOURCE_DIR}/modules  # JUCE headers from FetchContent
    ${CMAKE_SOURCE_DIR}/Source  # Your project headers
)

# Add common test dependencies (no find_package needed when using FetchContent)
add_subdirectory(core)