#pragma once

#include <JuceHeader.h>

namespace ForensEQ {

/**
 * Class for analyzing and comparing loudness metrics between tracks
 */
class LoudnessAnalyzer {
public:
    LoudnessAnalyzer();
    ~LoudnessAnalyzer();
    
    // Loudness measurement types
    enum class LoudnessType {
        Integrated,
        ShortTerm,
        Momentary,
        RMS
    };
    
    // Calculate loudness metrics for an audio buffer
    float calculateLoudness(const juce::AudioBuffer<float>& buffer, 
                           LoudnessType type, 
                           double sampleRate = 44100.0);
    
    // Calculate loudness difference between two audio buffers
    float calculateLoudnessDifference(const juce::AudioBuffer<float>& userBuffer,
                                     const juce::AudioBuffer<float>& referenceBuffer,
                                     LoudnessType type,
                                     double sampleRate = 44100.0);
    
    // Get a descriptive string for a loudness difference
    juce::String getLoudnessDifferenceDescription(float difference, LoudnessType type);
    
    // Get a color representing the severity of a loudness difference
    juce::Colour getLoudnessDifferenceColor(float difference, LoudnessType type);
    
    // Calculate match score (0.0 to 1.0) for loudness between user and reference
    float calculateLoudnessMatchScore(const juce::AudioBuffer<float>& userBuffer,
                                     const juce::AudioBuffer<float>& referenceBuffer,
                                     LoudnessType type,
                                     double sampleRate = 44100.0);

private:
    // Internal implementation methods
    float calculateIntegratedLUFS(const juce::AudioBuffer<float>& buffer, double sampleRate);
    float calculateShortTermLUFS(const juce::AudioBuffer<float>& buffer, double sampleRate);
    float calculateMomentaryLUFS(const juce::AudioBuffer<float>& buffer, double sampleRate);
    float calculateRMS(const juce::AudioBuffer<float>& buffer);
    
    // Tolerance thresholds for match scoring
    float perfectMatchThreshold = 0.5f;  // LUFS difference for 100% match
    float goodMatchThreshold = 2.0f;     // LUFS difference for 75% match
    float fairMatchThreshold = 5.0f;     // LUFS difference for 50% match
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoudnessAnalyzer)
};

} // namespace ForensEQ
