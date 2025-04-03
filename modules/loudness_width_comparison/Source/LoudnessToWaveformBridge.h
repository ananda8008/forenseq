#pragma once

#include <JuceHeader.h>
#include "ComparisonResult.h"
#include "LoudnessWidthAnalyzer.h"

namespace ForensEQ {

/**
 * Bridge class to connect the loudness/width module with the waveform viewer module
 */
class LoudnessToWaveformBridge {
public:
    LoudnessToWaveformBridge();
    ~LoudnessToWaveformBridge();
    
    // Set the comparison result to use for waveform data
    void setComparisonResult(const ComparisonResult& result);
    
    // Set the current stem type to analyze
    void setStemType(ComparisonResult::StemType type);
    
    // Get loudness markers for the waveform display
    // Returns time positions and loudness values that can be overlaid on the waveform
    struct LoudnessMarker {
        float timePosition;    // Time position in seconds
        float loudnessValue;   // Loudness value in LUFS
        bool isUserMix;        // Whether this marker is for user mix or reference
    };
    
    juce::Array<LoudnessMarker> getLoudnessMarkers() const;
    
    // Get width markers for the waveform display
    // Returns time positions and width values that can be overlaid on the waveform
    struct WidthMarker {
        float timePosition;    // Time position in seconds
        float widthValue;      // Width value as percentage
        bool isUserMix;        // Whether this marker is for user mix or reference
    };
    
    juce::Array<WidthMarker> getWidthMarkers() const;
    
    // Get regions where loudness or width significantly differs between user and reference
    // These can be highlighted in the waveform display
    struct DifferenceRegion {
        float startTime;       // Start time in seconds
        float endTime;         // End time in seconds
        float difference;      // Magnitude of difference
        bool isLoudness;       // Whether this is a loudness or width difference
    };
    
    juce::Array<DifferenceRegion> getDifferenceRegions() const;

private:
    ComparisonResult currentResult;
    ComparisonResult::StemType currentStemType = ComparisonResult::StemType::FullMix;
    
    // Mock data for demonstration purposes
    // In a real implementation, this would be derived from actual analysis
    juce::Array<LoudnessMarker> mockLoudnessMarkers;
    juce::Array<WidthMarker> mockWidthMarkers;
    juce::Array<DifferenceRegion> mockDifferenceRegions;
    
    // Helper method to generate mock data
    void generateMockData();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoudnessToWaveformBridge)
};

} // namespace ForensEQ
