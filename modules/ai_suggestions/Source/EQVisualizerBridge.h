#pragma once

#include <JuceHeader.h>
#include "SuggestionEngine.h"
#include "SuggestionAnalyzer.h"

namespace ForensEQ {

/**
 * Class that handles integration with the EQ visualizer module
 */
class EQVisualizerBridge {
public:
    EQVisualizerBridge();
    ~EQVisualizerBridge();
    
    // Set the suggestion analyzer to use
    void setSuggestionAnalyzer(SuggestionAnalyzer* analyzer) { suggestionAnalyzer = analyzer; }
    
    // Process EQ data from the EQ visualizer module
    void processEQData(const juce::Array<float>& userEQData, 
                      const juce::Array<float>& referenceEQData,
                      const juce::String& stemName);
    
    // Apply a suggestion to the EQ visualizer
    bool applySuggestion(const MixSuggestion& suggestion);
    
private:
    SuggestionAnalyzer* suggestionAnalyzer = nullptr;
    
    // Parse frequency from suggestion description
    float parseFrequencyFromDescription(const juce::String& description);
    
    // Parse dB value from suggestion description
    float parseDBValueFromDescription(const juce::String& description);
};

} // namespace ForensEQ
