#pragma once

#include <JuceHeader.h>
#include "ComparisonResult.h"
#include "LoudnessWidthAnalyzer.h"

namespace ForensEQ {

/**
 * Bridge class to connect the loudness/width module with the EQ visualizer module
 */
class LoudnessToEQBridge {
public:
    LoudnessToEQBridge();
    ~LoudnessToEQBridge();
    
    // Set the comparison result to use for EQ data
    void setComparisonResult(const ComparisonResult& result);
    
    // Set the current stem type to analyze
    void setStemType(ComparisonResult::StemType type);
    
    // Get EQ data for the current stem and comparison result
    // Returns frequency data that can be passed to the EQ visualizer
    juce::Array<float> getEQFrequencyData(bool isUserMix) const;
    
    // Get the match percentage between user and reference EQ curves
    float getEQMatchPercentage() const;

private:
    ComparisonResult currentResult;
    ComparisonResult::StemType currentStemType = ComparisonResult::StemType::FullMix;
    
    // Helper methods to convert loudness data to EQ frequency data
    juce::Array<float> generateEQDataFromLoudness(bool isUserMix) const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoudnessToEQBridge)
};

} // namespace ForensEQ
