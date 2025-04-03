#pragma once

#include <JuceHeader.h>
#include "SuggestionEngine.h"
#include "SuggestionAnalyzer.h"

namespace ForensEQ {

/**
 * Class that handles integration with the Loudness/Width Comparison module
 */
class LoudnessWidthBridge {
public:
    LoudnessWidthBridge();
    ~LoudnessWidthBridge();
    
    // Set the suggestion analyzer to use
    void setSuggestionAnalyzer(SuggestionAnalyzer* analyzer) { suggestionAnalyzer = analyzer; }
    
    // Process loudness data from the Loudness/Width Comparison module
    void processLoudnessData(float userLUFS, float referenceLUFS,
                           float userRMS, float referenceRMS,
                           const juce::String& stemName);
    
    // Process width data from the Loudness/Width Comparison module
    void processWidthData(float userWidth, float referenceWidth,
                        const juce::String& stemName);
    
    // Apply a suggestion to the Loudness/Width Comparison module
    bool applySuggestion(const MixSuggestion& suggestion);
    
private:
    SuggestionAnalyzer* suggestionAnalyzer = nullptr;
    
    // Helper method to parse loudness values from suggestion description
    float parseLoudnessFromDescription(const juce::String& description);
    
    // Helper method to parse width values from suggestion description
    float parseWidthFromDescription(const juce::String& description);
};

} // namespace ForensEQ
