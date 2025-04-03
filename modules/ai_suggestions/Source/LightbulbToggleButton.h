#pragma once

#include <JuceHeader.h>

namespace ForensEQ {

/**
 * A custom toggle button styled as a vintage Edison lightbulb
 */
class LightbulbToggleButton : public juce::Button,
                             public juce::Timer
{
public:
    LightbulbToggleButton();
    ~LightbulbToggleButton() override;

    // Button overrides
    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
    
    // Timer callback for animation
    void timerCallback() override;
    
    // Set the tooltip text
    void setTooltipText(const juce::String& activeText, const juce::String& inactiveText);
    
private:
    // Animation properties
    float glowIntensity = 0.0f;
    float glowPhase = 0.0f;
    bool glowIncreasing = true;
    
    // Tooltip text
    juce::String activeTooltip = "AI Suggestions Active";
    juce::String inactiveTooltip = "Suggestions Hidden";
    
    // Draw the bulb
    void drawBulb(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool isOn, float glowAmount);
    
    // Draw the filament
    void drawFilament(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool isOn, float glowAmount);
    
    // Draw the base
    void drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool isOn);
    
    // Draw the glow effect
    void drawGlow(juce::Graphics& g, const juce::Rectangle<float>& bounds, float intensity);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LightbulbToggleButton)
};

} // namespace ForensEQ
