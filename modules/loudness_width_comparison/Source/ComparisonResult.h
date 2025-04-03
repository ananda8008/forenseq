#pragma once

#include <JuceHeader.h>
#include "LoudnessAnalyzer.h"
#include "StereoWidthAnalyzer.h"

namespace ForensEQ {

/**
 * Class for storing and managing comparison results between user and reference tracks
 */
class ComparisonResult {
public:
    ComparisonResult();
    ~ComparisonResult();
    
    // Stem types that can be analyzed
    enum class StemType {
        FullMix,
        Kick,
        Snare,
        Bass,
        Vocals,
        Other
    };
    
    // Set loudness values for a specific stem
    void setLoudnessValues(StemType stemType, 
                          float userIntegratedLUFS, 
                          float referenceIntegratedLUFS,
                          float userShortTermLUFS,
                          float referenceShortTermLUFS,
                          float userMomentaryLUFS,
                          float referenceMomentaryLUFS,
                          float userRMS,
                          float referenceRMS);
    
    // Set width values for a specific stem
    void setWidthValues(StemType stemType,
                       float userCorrelation,
                       float referenceCorrelation,
                       float userMidSideRatio,
                       float referenceMidSideRatio);
    
    // Get loudness difference for a specific stem and loudness type
    float getLoudnessDifference(StemType stemType, LoudnessAnalyzer::LoudnessType loudnessType) const;
    
    // Get width difference for a specific stem and width type
    float getWidthDifference(StemType stemType, StereoWidthAnalyzer::WidthType widthType) const;
    
    // Get loudness match score for a specific stem and loudness type
    float getLoudnessMatchScore(StemType stemType, LoudnessAnalyzer::LoudnessType loudnessType) const;
    
    // Get width match score for a specific stem and width type
    float getWidthMatchScore(StemType stemType, StereoWidthAnalyzer::WidthType widthType) const;
    
    // Get combined match score for a specific stem (average of loudness and width)
    float getCombinedMatchScore(StemType stemType) const;
    
    // Get user loudness value for a specific stem and loudness type
    float getUserLoudness(StemType stemType, LoudnessAnalyzer::LoudnessType loudnessType) const;
    
    // Get reference loudness value for a specific stem and loudness type
    float getReferenceLoudness(StemType stemType, LoudnessAnalyzer::LoudnessType loudnessType) const;
    
    // Get user width value for a specific stem and width type
    float getUserWidth(StemType stemType, StereoWidthAnalyzer::WidthType widthType) const;
    
    // Get reference width value for a specific stem and width type
    float getReferenceWidth(StemType stemType, StereoWidthAnalyzer::WidthType widthType) const;
    
    // Convert to JSON string for data exchange
    juce::String toJSON() const;
    
    // Load from JSON string
    bool fromJSON(const juce::String& jsonString);

private:
    // Structure to hold loudness values for a stem
    struct LoudnessValues {
        float userIntegratedLUFS = -70.0f;
        float referenceIntegratedLUFS = -70.0f;
        float userShortTermLUFS = -70.0f;
        float referenceShortTermLUFS = -70.0f;
        float userMomentaryLUFS = -70.0f;
        float referenceMomentaryLUFS = -70.0f;
        float userRMS = -70.0f;
        float referenceRMS = -70.0f;
    };
    
    // Structure to hold width values for a stem
    struct WidthValues {
        float userCorrelation = 1.0f;
        float referenceCorrelation = 1.0f;
        float userMidSideRatio = 0.0f;
        float referenceMidSideRatio = 0.0f;
    };
    
    // Maps to store values for each stem type
    std::map<StemType, LoudnessValues> loudnessValues;
    std::map<StemType, WidthValues> widthValues;
    
    // Helper methods to calculate match scores
    float calculateLoudnessMatchScore(float difference, LoudnessAnalyzer::LoudnessType type) const;
    float calculateWidthMatchScore(float difference, StereoWidthAnalyzer::WidthType type) const;
    
    // Tolerance thresholds for match scoring
    float loudnessPerfectMatchThreshold = 0.5f;  // LUFS difference for 100% match
    float loudnessGoodMatchThreshold = 2.0f;     // LUFS difference for 75% match
    float loudnessFairMatchThreshold = 5.0f;     // LUFS difference for 50% match
    
    float widthPerfectMatchThreshold = 0.05f;    // Width difference for 100% match
    float widthGoodMatchThreshold = 0.15f;       // Width difference for 75% match
    float widthFairMatchThreshold = 0.3f;        // Width difference for 50% match
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComparisonResult)
};

} // namespace ForensEQ
