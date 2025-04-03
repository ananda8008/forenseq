#pragma once

#include <JuceHeader.h>
#include "SuggestionEngine.h"
#include "SuggestionAnalyzer.h"

namespace ForensEQ {

/**
 * Class that handles integration with the Stem Analysis module
 */
class StemAnalysisBridge {
public:
    StemAnalysisBridge();
    ~StemAnalysisBridge();
    
    // Set the suggestion analyzer to use
    void setSuggestionAnalyzer(SuggestionAnalyzer* analyzer) { suggestionAnalyzer = analyzer; }
    
    // Process stem data from the Stem Analysis module
    void processStemData(const juce::var& stemData);
    
    // Apply a suggestion to the Stem Analysis module
    bool applySuggestion(const MixSuggestion& suggestion);
    
    // Get the current active stem
    juce::String getActiveStem() const { return activeStem; }
    
    // Set the current active stem
    void setActiveStem(const juce::String& stemName) { activeStem = stemName; }
    
private:
    SuggestionAnalyzer* suggestionAnalyzer = nullptr;
    juce::String activeStem = "Full Mix";
    
    // Helper method to extract stem-specific data
    juce::var extractStemData(const juce::var& allStemData, const juce::String& stemName);
};

} // namespace ForensEQ
