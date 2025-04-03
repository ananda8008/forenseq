#pragma once

#include <JuceHeader.h>
#include "LoudnessWidthAnalyzer.h"

namespace ForensEQ {

/**
 * Component for displaying a loudness meter with comparison between user and reference
 */
class LoudnessMeterComponent : public juce::Component,
                              public juce::Timer
{
public:
    LoudnessMeterComponent();
    ~LoudnessMeterComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Set the loudness values to display
    void setLoudnessValues(float userLoudness, float referenceLoudness, 
                          LoudnessAnalyzer::LoudnessType type);
    
    // Set the stem type being displayed
    void setStemType(ComparisonResult::StemType type);
    
    // Set the loudness analyzer for color and text generation
    void setLoudnessAnalyzer(LoudnessAnalyzer* analyzer);
    
    // Timer callback for animations
    void timerCallback() override;

private:
    float userLoudness = -70.0f;
    float referenceLoudness = -70.0f;
    float displayUserLoudness = -70.0f;    // For smooth animation
    float displayReferenceLoudness = -70.0f; // For smooth animation
    
    LoudnessAnalyzer::LoudnessType loudnessType = LoudnessAnalyzer::LoudnessType::Integrated;
    ComparisonResult::StemType stemType = ComparisonResult::StemType::FullMix;
    
    LoudnessAnalyzer* loudnessAnalyzer = nullptr;
    
    // UI colors
    juce::Colour backgroundColor = juce::Colour(30, 30, 30);
    juce::Colour userColor = juce::Colour(0, 120, 255);
    juce::Colour referenceColor = juce::Colour(255, 160, 0);
    juce::Colour textColor = juce::Colour(220, 220, 220);
    juce::Colour meterBackgroundColor = juce::Colour(40, 40, 40);
    
    // UI settings
    float meterMinDb = -70.0f;
    float meterMaxDb = 0.0f;
    
    // Helper methods
    float dbToY(float db, float height) const;
    juce::String getLoudnessTypeString() const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoudnessMeterComponent)
};

} // namespace ForensEQ
