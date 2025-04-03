#pragma once

#include <JuceHeader.h>
#include "LoudnessAnalyzer.h"
#include "StereoWidthAnalyzer.h"
#include "ComparisonResult.h"

namespace ForensEQ {

/**
 * Class for analyzing loudness and stereo width of audio tracks and stems
 */
class LoudnessWidthAnalyzer {
public:
    LoudnessWidthAnalyzer();
    ~LoudnessWidthAnalyzer();
    
    // Analyze a user mix and reference track, comparing all stems
    ComparisonResult analyzeAndCompare(
        const juce::AudioBuffer<float>& userMix,
        const juce::AudioBuffer<float>& referenceMix,
        const std::map<ComparisonResult::StemType, juce::AudioBuffer<float>>& userStems,
        const std::map<ComparisonResult::StemType, juce::AudioBuffer<float>>& referenceStems,
        double sampleRate = 44100.0);
    
    // Analyze a single audio buffer for loudness and width
    void analyzeSingleTrack(
        const juce::AudioBuffer<float>& buffer,
        float& integratedLUFS,
        float& shortTermLUFS,
        float& momentaryLUFS,
        float& rms,
        float& correlation,
        float& midSideRatio,
        double sampleRate = 44100.0);
    
    // Get the loudness analyzer
    LoudnessAnalyzer& getLoudnessAnalyzer() { return loudnessAnalyzer; }
    
    // Get the stereo width analyzer
    StereoWidthAnalyzer& getStereoWidthAnalyzer() { return stereoWidthAnalyzer; }

private:
    LoudnessAnalyzer loudnessAnalyzer;
    StereoWidthAnalyzer stereoWidthAnalyzer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoudnessWidthAnalyzer)
};

} // namespace ForensEQ
