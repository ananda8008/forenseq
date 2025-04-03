#pragma once

#include <JuceHeader.h>
#include "SuggestionEngine.h"

namespace ForensEQ {

/**
 * Class that handles integration with other ForensEQ modules to gather data for suggestion generation
 */
class SuggestionAnalyzer {
public:
    SuggestionAnalyzer();
    ~SuggestionAnalyzer();
    
    // Set the suggestion engine to use
    void setSuggestionEngine(SuggestionEngine* engine) { suggestionEngine = engine; }
    
    // Analyze EQ data from the EQ visualizer module
    void analyzeEQData(const juce::Array<float>& userEQData, 
                      const juce::Array<float>& referenceEQData,
                      const juce::String& stemName);
    
    // Analyze loudness data from the loudness/width comparison module
    void analyzeLoudnessData(float userLUFS, float referenceLUFS,
                            float userRMS, float referenceRMS,
                            const juce::String& stemName);
    
    // Analyze stereo width data from the loudness/width comparison module
    void analyzeWidthData(float userWidth, float referenceWidth,
                         const juce::String& stemName);
    
    // Analyze all stems using data from the stem analysis module
    void analyzeAllStems(const juce::var& stemAnalysisData);
    
    // Generate advanced suggestions based on combined analysis
    void generateAdvancedSuggestions();
    
private:
    SuggestionEngine* suggestionEngine = nullptr;
    
    // Store analysis data for advanced suggestion generation
    struct StemAnalysisData {
        juce::String stemName;
        juce::Array<float> userEQData;
        juce::Array<float> referenceEQData;
        float userLUFS = 0.0f;
        float referenceLUFS = 0.0f;
        float userRMS = 0.0f;
        float referenceRMS = 0.0f;
        float userWidth = 0.0f;
        float referenceWidth = 0.0f;
    };
    
    juce::Array<StemAnalysisData> stemAnalysisData;
    
    // Helper methods for advanced suggestion generation
    void generateFrequencyMaskingSuggestions();
    void generateDynamicBalanceSuggestions();
    void generateStereoImagingSuggestions();
    
    // Helper method to find a stem by name
    int findStemIndex(const juce::String& stemName);
};

} // namespace ForensEQ
