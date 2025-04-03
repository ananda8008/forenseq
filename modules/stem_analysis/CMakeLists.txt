cmake_minimum_required(VERSION 3.15)

project(ForensEQ_StemAnalysis VERSION 0.1.0)

# Add JUCE as a subdirectory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../JUCE ${CMAKE_BINARY_DIR}/JUCE EXCLUDE_FROM_ALL)

# Initialize JUCE
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/Source)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create a static library for the Stem Analysis module
juce_add_module(
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ALIAS ForensEQ_StemAnalysis
)

# Set module properties
set_target_properties(ForensEQ_StemAnalysis PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE
    FOLDER "Modules"
)

# Link with JUCE modules
target_link_libraries(ForensEQ_StemAnalysis
    PUBLIC
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
)

# Add include directories
target_include_directories(ForensEQ_StemAnalysis
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Create a simple test application to demonstrate the Stem Analysis
juce_add_gui_app(ForensEQ_StemAnalysis_Demo
    PRODUCT_NAME "ForensEQ Stem Analysis Demo"
    COMPANY_NAME "ForensEQ"
)

# Add source files for the demo app
target_sources(ForensEQ_StemAnalysis_Demo
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/Demo/Main.cpp
)

# Link the demo app with our module
target_link_libraries(ForensEQ_StemAnalysis_Demo
    PRIVATE
        ForensEQ_StemAnalysis
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
)

# Set startup project
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ForensEQ_StemAnalysis_Demo)
