# Professional test target creation macro
function(add_juce_test_target TARGET_NAME)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBS" ${ARGN})

    add_executable(${TARGET_NAME} ${ARG_SOURCES})
    
    target_link_libraries(${TARGET_NAME} PRIVATE
        ${ARG_LIBS}
        juce_unit_test
    )
    
    target_compile_definitions(${TARGET_NAME} PRIVATE
        JUCE_MODAL_LOOPS_PERMITTED=1
        JUCE_DISPLAY_SPLASH_SCREEN=0
    )
    
    # Professional test discovery
    add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
    
    # Code coverage setup (when enabled)
    if(CODE_COVERAGE)
        target_compile_options(${TARGET_NAME} PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(${TARGET_NAME} PRIVATE --coverage)
    endif()
endfunction()