#pragma once

#include <JuceHeader.h>
#include "ComparisonResult.h"

namespace ForensEQ {

/**
 * Component for displaying match scores between user and reference tracks
 */
class MatchScoreComponent : public juce::Component
{
public:
    MatchScoreComponent();
    ~MatchScoreComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Set the match scores to display
    void setMatchScores(float loudnessScore, float widthScore, float combinedScore);
    
    // Set the stem type being displayed
    void setStemType(ComparisonResult::StemType type);

private:
    float loudnessScore = 0.0f;
    float widthScore = 0.0f;
    float combinedScore = 0.0f;
    
    ComparisonResult::StemType stemType = ComparisonResult::StemType::FullMix;
    
    // UI colors
    juce::Colour backgroundColor = juce::Colour(30, 30, 30);
    juce::Colour textColor = juce::Colour(220, 220, 220);
    juce::Colour loudnessColor = juce::Colour(0, 180, 255);
    juce::Colour widthColor = juce::Colour(255, 160, 0);
    juce::Colour combinedColor = juce::Colour(180, 255, 140);
    
    // Helper methods
    juce::String getStemTypeString() const;
    juce::Colour getScoreColor(float score) const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MatchScoreComponent)
};

} // namespace ForensEQ
