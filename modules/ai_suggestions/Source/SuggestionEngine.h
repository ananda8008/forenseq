#pragma once

#include <JuceHeader.h>
#include "MixSuggestion.h"
#include "SuggestionManager.h"

namespace ForensEQ {

/**
 * Class that generates mix suggestions based on analysis data
 */
class SuggestionEngine {
public:
    SuggestionEngine();
    ~SuggestionEngine();
    
    // Generate suggestions based on EQ data
    void generateEQSuggestions(const juce::Array<float>& userEQData, 
                              const juce::Array<float>& referenceEQData,
                              const juce::String& stemName);
    
    // Generate suggestions based on loudness data
    void generateLoudnessSuggestions(float userLUFS, float referenceLUFS,
                                    float userRMS, float referenceRMS,
                                    const juce::String& stemName);
    
    // Generate suggestions based on stereo width data
    void generateWidthSuggestions(float userWidth, float referenceWidth,
                                 const juce::String& stemName);
    
    // Get the suggestion manager containing all generated suggestions
    SuggestionManager& getSuggestionManager() { return suggestionManager; }
    
    // Clear all suggestions
    void clearSuggestions() { suggestionManager.clearSuggestions(); }
    
    // Set threshold values for suggestion generation
    void setEQThreshold(float threshold) { eqThreshold = threshold; }
    void setLoudnessThreshold(float threshold) { loudnessThreshold = threshold; }
    void setWidthThreshold(float threshold) { widthThreshold = threshold; }
    
private:
    SuggestionManager suggestionManager;
    
    // Threshold values for suggestion generation
    float eqThreshold = 3.0f;         // dB difference to trigger EQ suggestions
    float loudnessThreshold = 1.0f;   // LUFS difference to trigger loudness suggestions
    float widthThreshold = 10.0f;     // Percentage difference to trigger width suggestions
    
    // Helper methods for suggestion generation
    juce::String generateEQDescription(float frequency, float difference);
    juce::String generateLoudnessDescription(float lufsDifference, float rmsDifference);
    juce::String generateWidthDescription(float widthDifference);
    
    // Helper method to determine suggestion priority based on difference magnitude
    SuggestionPriority getPriorityFromDifference(float difference, float threshold);
    
    // Frequency bands for EQ analysis (in Hz)
    const juce::Array<float> frequencyBands = {
        20, 30, 40, 50, 60, 80, 100, 120, 150, 180, 
        200, 250, 300, 350, 400, 500, 600, 700, 800, 
        1000, 1200, 1500, 1800, 2000, 2500, 3000, 3500, 
        4000, 5000, 6000, 8000, 10000, 12000, 16000, 20000
    };
};

} // namespace ForensEQ
