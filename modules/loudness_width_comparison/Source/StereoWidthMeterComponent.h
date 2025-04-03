#pragma once

#include <JuceHeader.h>
#include "StereoWidthAnalyzer.h"
#include "LoudnessWidthAnalyzer.h"

namespace ForensEQ {

/**
 * Component for displaying a stereo width meter with comparison between user and reference
 */
class StereoWidthMeterComponent : public juce::Component,
                                 public juce::Timer
{
public:
    StereoWidthMeterComponent();
    ~StereoWidthMeterComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Set the width values to display
    void setWidthValues(float userWidth, float referenceWidth, 
                       StereoWidthAnalyzer::WidthType type);
    
    // Set the stem type being displayed
    void setStemType(ComparisonResult::StemType type);
    
    // Set the width analyzer for color and text generation
    void setWidthAnalyzer(StereoWidthAnalyzer* analyzer);
    
    // Timer callback for animations
    void timerCallback() override;

private:
    float userWidth = 0.0f;
    float referenceWidth = 0.0f;
    float displayUserWidth = 0.0f;       // For smooth animation
    float displayReferenceWidth = 0.0f;  // For smooth animation
    
    StereoWidthAnalyzer::WidthType widthType = StereoWidthAnalyzer::WidthType::Percentage;
    ComparisonResult::StemType stemType = ComparisonResult::StemType::FullMix;
    
    StereoWidthAnalyzer* widthAnalyzer = nullptr;
    
    // UI colors
    juce::Colour backgroundColor = juce::Colour(30, 30, 30);
    juce::Colour userColor = juce::Colour(0, 120, 255);
    juce::Colour referenceColor = juce::Colour(255, 160, 0);
    juce::Colour textColor = juce::Colour(220, 220, 220);
    juce::Colour meterBackgroundColor = juce::Colour(40, 40, 40);
    juce::Colour glowColor = juce::Colour(255, 255, 255);
    
    // Glow effect parameters
    float glowIntensity = 0.0f;
    float glowPhase = 0.0f;
    
    // Helper methods
    float widthToX(float width, float meterWidth) const;
    juce::String getWidthTypeString() const;
    void drawGlowEffect(juce::Graphics& g, juce::Rectangle<int> bounds, float intensity);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StereoWidthMeterComponent)
};

} // namespace ForensEQ
