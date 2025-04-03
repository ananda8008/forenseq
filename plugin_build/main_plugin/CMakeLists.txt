cmake_minimum_required(VERSION 3.15)

# Project name and version
project(ForensEQ VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE as a subdirectory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce JUCE)

# Include JUCE modules
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce/modules/juce_audio_basics)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce/modules/juce_audio_devices)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce/modules/juce_audio_formats)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce/modules/juce_audio_plugin_client)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce/modules/juce_audio_processors)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce/modules/juce_audio_utils)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce/modules/juce_core)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce/modules/juce_data_structures)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce/modules/juce_dsp)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce/modules/juce_events)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce/modules/juce_graphics)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce/modules/juce_gui_basics)
juce_add_module(${CMAKE_CURRENT_SOURCE_DIR}/../../modules/juce/modules/juce_gui_extra)

# Set plugin properties
juce_add_plugin(ForensEQ
    VERSION 1.0.0
    COMPANY_NAME "ForensEQ"
    COMPANY_WEBSITE "https://forenseq.com"
    COMPANY_EMAIL "info@forenseq.com"
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "ForensEQ"
    DESCRIPTION "Audio forensics and analysis plugin"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

# Include directories for all modules
target_include_directories(ForensEQ
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/ai_suggestions/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/eq_visualizer/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/loudness_width_comparison/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/stem_analysis/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/waveform_viewer/Source
)

# Add source files
file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h
)

# Add module source files
file(GLOB_RECURSE AI_SUGGESTIONS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/ai_suggestions/Source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/ai_suggestions/Source/*.h
)

file(GLOB_RECURSE EQ_VISUALIZER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/eq_visualizer/Source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/eq_visualizer/Source/*.h
)

file(GLOB_RECURSE LOUDNESS_WIDTH_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/loudness_width_comparison/Source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/loudness_width_comparison/Source/*.h
)

file(GLOB_RECURSE STEM_ANALYSIS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/stem_analysis/Source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/stem_analysis/Source/*.h
)

file(GLOB_RECURSE WAVEFORM_VIEWER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/waveform_viewer/Source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/waveform_viewer/Source/*.h
)

# Filter out test and demo files
list(FILTER AI_SUGGESTIONS_SOURCES EXCLUDE REGEX ".*Test/.*|.*Demo/.*")
list(FILTER EQ_VISUALIZER_SOURCES EXCLUDE REGEX ".*Test/.*|.*Demo/.*")
list(FILTER LOUDNESS_WIDTH_SOURCES EXCLUDE REGEX ".*Test/.*|.*Demo/.*")
list(FILTER STEM_ANALYSIS_SOURCES EXCLUDE REGEX ".*Test/.*|.*Demo/.*")
list(FILTER WAVEFORM_VIEWER_SOURCES EXCLUDE REGEX ".*Test/.*|.*Demo/.*")

# Add all sources to the plugin
target_sources(ForensEQ
    PRIVATE
    ${SOURCES}
    ${AI_SUGGESTIONS_SOURCES}
    ${EQ_VISUALIZER_SOURCES}
    ${LOUDNESS_WIDTH_SOURCES}
    ${STEM_ANALYSIS_SOURCES}
    ${WAVEFORM_VIEWER_SOURCES}
)

# Link JUCE modules
target_link_libraries(ForensEQ
    PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

# Set compile definitions
target_compile_definitions(ForensEQ
    PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
)

# Set binary output directory
set_target_properties(ForensEQ PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Add resources
juce_add_binary_data(ForensEQResources
    SOURCES
    # Add resource files here if needed
)

# Link resources
target_link_libraries(ForensEQ PRIVATE ForensEQResources)
