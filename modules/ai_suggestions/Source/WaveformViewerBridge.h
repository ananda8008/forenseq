#pragma once

#include <JuceHeader.h>
#include "SuggestionEngine.h"
#include "SuggestionAnalyzer.h"

namespace ForensEQ {

/**
 * Class that handles integration with the Waveform Viewer module
 */
class WaveformViewerBridge {
public:
    WaveformViewerBridge();
    ~WaveformViewerBridge();
    
    // Set the suggestion analyzer to use
    void setSuggestionAnalyzer(SuggestionAnalyzer* analyzer) { suggestionAnalyzer = analyzer; }
    
    // Process waveform data from the Waveform Viewer module
    void processWaveformData(const juce::AudioBuffer<float>& userAudio, 
                           const juce::AudioBuffer<float>& referenceAudio,
                           const juce::String& stemName);
    
    // Apply a suggestion to the Waveform Viewer
    bool applySuggestion(const MixSuggestion& suggestion);
    
private:
    SuggestionAnalyzer* suggestionAnalyzer = nullptr;
    
    // Helper methods for waveform analysis
    void analyzeTransients(const juce::AudioBuffer<float>& audio, const juce::String& stemName);
    void analyzeTimingAlignment(const juce::AudioBuffer<float>& userAudio, 
                              const juce::AudioBuffer<float>& referenceAudio,
                              const juce::String& stemName);
};

} // namespace ForensEQ
