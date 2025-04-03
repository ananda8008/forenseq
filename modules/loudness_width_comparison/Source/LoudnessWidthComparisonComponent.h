#pragma once

#include <JuceHeader.h>
#include "LoudnessMeterComponent.h"
#include "StereoWidthMeterComponent.h"
#include "MatchScoreComponent.h"
#include "ComparisonResult.h"
#include "LoudnessWidthAnalyzer.h"

namespace ForensEQ {

/**
 * Main component that integrates all loudness and width comparison UI elements
 */
class LoudnessWidthComparisonComponent : public juce::Component
{
public:
    LoudnessWidthComparisonComponent();
    ~LoudnessWidthComparisonComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Set the comparison result to display
    void setComparisonResult(const ComparisonResult& result);
    
    // Set the stem type to display
    void setStemType(ComparisonResult::StemType type);
    
    // Get the current stem type
    ComparisonResult::StemType getStemType() const { return currentStemType; }
    
    // Set the analyzers for text and color generation
    void setAnalyzers(LoudnessAnalyzer* loudnessAnalyzer, StereoWidthAnalyzer* widthAnalyzer);

private:
    // UI components
    LoudnessMeterComponent integratedLUFSMeter;
    LoudnessMeterComponent shortTermLUFSMeter;
    LoudnessMeterComponent momentaryLUFSMeter;
    LoudnessMeterComponent rmsMeter;
    
    StereoWidthMeterComponent widthMeter;
    MatchScoreComponent matchScoreDisplay;
    
    // Current state
    ComparisonResult currentResult;
    ComparisonResult::StemType currentStemType = ComparisonResult::StemType::FullMix;
    
    // UI colors
    juce::Colour backgroundColor = juce::Colour(20, 20, 20);
    juce::Colour textColor = juce::Colour(220, 220, 220);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoudnessWidthComparisonComponent)
};

} // namespace ForensEQ
