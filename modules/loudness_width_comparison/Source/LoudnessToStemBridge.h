#pragma once

#include <JuceHeader.h>
#include "ComparisonResult.h"
#include "LoudnessWidthAnalyzer.h"

namespace ForensEQ {

/**
 * Bridge class to connect the loudness/width module with the stem analysis module
 */
class LoudnessToStemBridge {
public:
    LoudnessToStemBridge();
    ~LoudnessToStemBridge();
    
    // Set the comparison result to use for stem data
    void setComparisonResult(const ComparisonResult& result);
    
    // Get loudness data for all stems
    struct StemLoudnessData {
        ComparisonResult::StemType stemType;
        float userIntegratedLUFS;
        float referenceIntegratedLUFS;
        float loudnessDifference;
        float matchScore;
    };
    
    juce::Array<StemLoudnessData> getAllStemLoudnessData() const;
    
    // Get width data for all stems
    struct StemWidthData {
        ComparisonResult::StemType stemType;
        float userWidthPercentage;
        float referenceWidthPercentage;
        float widthDifference;
        float matchScore;
    };
    
    juce::Array<StemWidthData> getAllStemWidthData() const;
    
    // Get combined match scores for all stems
    struct StemMatchData {
        ComparisonResult::StemType stemType;
        float combinedMatchScore;
        juce::String matchDescription;
    };
    
    juce::Array<StemMatchData> getAllStemMatchData() const;
    
    // Get JSON data for AI suggestions module (future integration)
    juce::String getAISuggestionData() const;

private:
    ComparisonResult currentResult;
    
    // Helper methods
    juce::String getStemTypeName(ComparisonResult::StemType type) const;
    juce::String getMatchDescription(float score) const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoudnessToStemBridge)
};

} // namespace ForensEQ
