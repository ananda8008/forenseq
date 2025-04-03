#pragma once

#include <JuceHeader.h>

/**
 * ForensEQTheme - Unified theme class for the ForensEQ plugin
 * 
 * Provides consistent dark mode styling with subtle illumination text effects
 * across all modules of the ForensEQ plugin.
 */
class ForensEQTheme : public juce::LookAndFeel_V4
{
public:
    ForensEQTheme();
    ~ForensEQTheme() override = default;
    
    // Singleton instance
    static ForensEQTheme& getInstance();
    
    // Color getters for consistent styling across components
    juce::Colour getBackgroundColor() const { return backgroundColour; }
    juce::Colour getBackgroundAccentColor() const { return backgroundAccentColour; }
    juce::Colour getPrimaryColor() const { return primaryColour; }
    juce::Colour getSecondaryColor() const { return secondaryColour; }
    juce::Colour getHighlightColor() const { return highlightColour; }
    juce::Colour getTextColor() const { return textColour; }
    juce::Colour getTextHighlightColor() const { return textHighlightColour; }
    juce::Colour getWarningColor() const { return warningColour; }
    juce::Colour getSuccessColor() const { return successColour; }
    juce::Colour getWaveformColor() const { return waveformColour; }
    juce::Colour getPlayPositionColor() const { return playPositionColour; }
    juce::Colour getSelectionColor() const { return selectionColour; }
    
    // Illumination effect methods
    juce::Colour getIlluminatedColor(juce::Colour baseColor, float intensity = 1.0f) const;
    juce::DropShadow getTextGlow(juce::Colour glowColor, float intensity = 1.0f) const;
    
    // Override JUCE LookAndFeel methods for consistent styling
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, 
                             const juce::Colour& backgroundColour,
                             bool shouldDrawButtonAsHighlighted, 
                             bool shouldDrawButtonAsDown) override;
                             
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                         float sliderPosProportional, float rotaryStartAngle,
                         float rotaryEndAngle, juce::Slider& slider) override;
                         
    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                         float sliderPos, float minSliderPos, float maxSliderPos,
                         const juce::Slider::SliderStyle style, juce::Slider& slider) override;
                         
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                         bool shouldDrawButtonAsHighlighted, 
                         bool shouldDrawButtonAsDown) override;
                         
    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                     int buttonX, int buttonY, int buttonW, int buttonH,
                     juce::ComboBox& box) override;
                     
    void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
                          bool isSeparator, bool isActive, bool isHighlighted,
                          bool isTicked, bool hasSubMenu, const juce::String& text,
                          const juce::String& shortcutKeyText, const juce::Drawable* icon,
                          const juce::Colour* textColour) override;
                          
    juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override;
    
private:
    // Core theme colors
    juce::Colour backgroundColour;      // Dark background (20, 20, 22)
    juce::Colour backgroundAccentColour; // Slightly lighter background (30, 30, 34)
    juce::Colour primaryColour;         // Primary accent (0, 120, 255) - Blue
    juce::Colour secondaryColour;       // Secondary accent (255, 120, 0) - Orange
    juce::Colour highlightColour;       // Highlight (255, 255, 200) - Light gold
    juce::Colour textColour;            // Main text (220, 220, 220) - Light gray
    juce::Colour textHighlightColour;   // Highlighted text (255, 255, 255) - White
    juce::Colour warningColour;         // Warning (255, 60, 60) - Red
    juce::Colour successColour;         // Success (60, 255, 120) - Green
    
    // Module-specific colors
    juce::Colour waveformColour;        // Waveform (0, 120, 255) - Blue
    juce::Colour playPositionColour;    // Playback position (255, 255, 200) - Light gold
    juce::Colour selectionColour;       // Selection (255, 120, 0) - Orange
    
    // Singleton instance
    static ForensEQTheme* instance;
};
