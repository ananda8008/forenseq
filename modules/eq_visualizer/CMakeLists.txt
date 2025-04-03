cmake_minimum_required(VERSION 3.15)

project(ForensEQ_EQVisualizer VERSION 0.1.0)

# Add JUCE as a subdirectory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../JUCE ${CMAKE_BINARY_DIR}/JUCE EXCLUDE_FROM_ALL)

# Initialize JUCE
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/Source)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create a static library for the EQ Visualizer module
juce_add_module(
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ALIAS ForensEQ_EQVisualizer
)

# Set module properties
set_target_properties(ForensEQ_EQVisualizer PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE
    FOLDER "Modules"
)

# Link with JUCE modules
target_link_libraries(ForensEQ_EQVisualizer
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
target_include_directories(ForensEQ_EQVisualizer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Create a simple test application to demonstrate the EQ Visualizer
juce_add_gui_app(ForensEQ_EQVisualizer_Demo
    PRODUCT_NAME "ForensEQ EQ Visualizer Demo"
    COMPANY_NAME "ForensEQ"
)

# Add source files for the demo app
target_sources(ForensEQ_EQVisualizer_Demo
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/Demo/Main.cpp
)

# Link the demo app with our module
target_link_libraries(ForensEQ_EQVisualizer_Demo
    PRIVATE
        ForensEQ_EQVisualizer
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
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ForensEQ_EQVisualizer_Demo)
