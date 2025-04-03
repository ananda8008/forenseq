#pragma once

#include <JuceHeader.h>

namespace ForensEQ {

/**
 * Class for analyzing and comparing stereo width metrics between tracks
 */
class StereoWidthAnalyzer {
public:
    StereoWidthAnalyzer();
    ~StereoWidthAnalyzer();
    
    // Width measurement types
    enum class WidthType {
        Correlation,  // Phase correlation coefficient (-1 to 1)
        MidSideRatio, // Ratio of side level to mid level (0 to inf)
        Percentage    // Normalized width percentage (0 to 100%)
    };
    
    // Calculate stereo width for an audio buffer
    float calculateWidth(const juce::AudioBuffer<float>& buffer, WidthType type);
    
    // Calculate width difference between two audio buffers
    float calculateWidthDifference(const juce::AudioBuffer<float>& userBuffer,
                                  const juce::AudioBuffer<float>& referenceBuffer,
                                  WidthType type);
    
    // Get a descriptive string for a width difference
    juce::String getWidthDifferenceDescription(float difference, WidthType type);
    
    // Get a color representing the severity of a width difference
    juce::Colour getWidthDifferenceColor(float difference, WidthType type);
    
    // Calculate match score (0.0 to 1.0) for width between user and reference
    float calculateWidthMatchScore(const juce::AudioBuffer<float>& userBuffer,
                                  const juce::AudioBuffer<float>& referenceBuffer,
                                  WidthType type);
    
    // Convert correlation coefficient to percentage width (0-100%)
    float correlationToPercentage(float correlation);
    
    // Convert mid/side ratio to percentage width (0-100%)
    float midSideRatioToPercentage(float ratio);

private:
    // Internal implementation methods
    float calculateCorrelation(const juce::AudioBuffer<float>& buffer);
    float calculateMidSideRatio(const juce::AudioBuffer<float>& buffer);
    
    // Calculate mid and side levels
    void calculateMidSide(const juce::AudioBuffer<float>& buffer, 
                         float& midLevel, 
                         float& sideLevel);
    
    // Tolerance thresholds for match scoring
    float perfectMatchThreshold = 0.05f;  // Width difference for 100% match
    float goodMatchThreshold = 0.15f;     // Width difference for 75% match
    float fairMatchThreshold = 0.3f;      // Width difference for 50% match
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereoWidthAnalyzer)
};

} // namespace ForensEQ
