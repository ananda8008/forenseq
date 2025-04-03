#include "ForensEQTheme.h"

// Initialize static instance
ForensEQTheme* ForensEQTheme::instance = nullptr;

ForensEQTheme::ForensEQTheme()
{
    // Initialize core theme colors
    backgroundColour = juce::Colour(20, 20, 22);
    backgroundAccentColour = juce::Colour(30, 30, 34);
    primaryColour = juce::Colour(0, 120, 255);      // Blue
    secondaryColour = juce::Colour(255, 120, 0);    // Orange
    highlightColour = juce::Colour(255, 255, 200);  // Light gold
    textColour = juce::Colour(220, 220, 220);       // Light gray
    textHighlightColour = juce::Colour(255, 255, 255); // White
    warningColour = juce::Colour(255, 60, 60);      // Red
    successColour = juce::Colour(60, 255, 120);     // Green
    
    // Module-specific colors
    waveformColour = primaryColour;                 // Blue
    playPositionColour = highlightColour;           // Light gold
    selectionColour = secondaryColour;              // Orange
    
    // Set default colors for JUCE components
    setColour(juce::ResizableWindow::backgroundColourId, backgroundColour);
    setColour(juce::TextButton::buttonColourId, backgroundAccentColour);
    setColour(juce::TextButton::textColourOffId, textColour);
    setColour(juce::TextButton::textColourOnId, textHighlightColour);
    
    setColour(juce::Slider::backgroundColourId, backgroundAccentColour);
    setColour(juce::Slider::thumbColourId, primaryColour);
    setColour(juce::Slider::trackColourId, primaryColour.withAlpha(0.6f));
    setColour(juce::Slider::textBoxTextColourId, textColour);
    setColour(juce::Slider::textBoxOutlineColourId, backgroundAccentColour);
    
    setColour(juce::ComboBox::backgroundColourId, backgroundAccentColour);
    setColour(juce::ComboBox::textColourId, textColour);
    setColour(juce::ComboBox::outlineColourId, primaryColour);
    setColour(juce::ComboBox::buttonColourId, primaryColour);
    setColour(juce::ComboBox::arrowColourId, textColour);
    
    setColour(juce::PopupMenu::backgroundColourId, backgroundColour);
    setColour(juce::PopupMenu::textColourId, textColour);
    setColour(juce::PopupMenu::highlightedBackgroundColourId, primaryColour);
    setColour(juce::PopupMenu::highlightedTextColourId, textHighlightColour);
    
    setColour(juce::Label::textColourId, textColour);
    setColour(juce::Label::outlineColourId, juce::Colours::transparentBlack);
    
    setColour(juce::TextEditor::backgroundColourId, backgroundAccentColour);
    setColour(juce::TextEditor::textColourId, textColour);
    setColour(juce::TextEditor::highlightColourId, primaryColour.withAlpha(0.3f));
    setColour(juce::TextEditor::highlightedTextColourId, textHighlightColour);
    setColour(juce::TextEditor::outlineColourId, primaryColour.withAlpha(0.5f));
    setColour(juce::TextEditor::focusedOutlineColourId, primaryColour);
    
    setColour(juce::ToggleButton::textColourId, textColour);
    setColour(juce::ToggleButton::tickColourId, primaryColour);
    setColour(juce::ToggleButton::tickDisabledColourId, primaryColour.withAlpha(0.4f));
}

ForensEQTheme& ForensEQTheme::getInstance()
{
    if (instance == nullptr)
        instance = new ForensEQTheme();
    
    return *instance;
}

juce::Colour ForensEQTheme::getIlluminatedColor(juce::Colour baseColor, float intensity) const
{
    // Create a glowing effect by brightening the color and increasing saturation
    float h, s, b;
    baseColor.getHSB(h, s, b);
    
    // Increase brightness and saturation for illumination effect
    float newBrightness = juce::jmin(1.0f, b + (0.2f * intensity));
    float newSaturation = juce::jmin(1.0f, s + (0.1f * intensity));
    
    return juce::Colour::fromHSV(h, newSaturation, newBrightness, baseColor.getFloatAlpha());
}

juce::DropShadow ForensEQTheme::getTextGlow(juce::Colour glowColor, float intensity) const
{
    // Create a subtle glow effect for text
    juce::DropShadow glow;
    glow.colour = glowColor.withAlpha(0.7f * intensity);
    glow.radius = 8 * intensity;
    glow.offset = juce::Point<int>(0, 0);
    
    return glow;
}

void ForensEQTheme::drawButtonBackground(juce::Graphics& g, juce::Button& button, 
                                       const juce::Colour& backgroundColour,
                                       bool shouldDrawButtonAsHighlighted, 
                                       bool shouldDrawButtonAsDown)
{
    juce::Rectangle<float> bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);
    float cornerSize = 4.0f;
    
    juce::Colour baseColour = backgroundColour;
    
    if (shouldDrawButtonAsDown)
        baseColour = baseColour.darker(0.2f);
    else if (shouldDrawButtonAsHighlighted)
        baseColour = baseColour.brighter(0.1f);
    
    g.setColour(baseColour);
    g.fillRoundedRectangle(bounds, cornerSize);
    
    // Add subtle gradient
    juce::ColourGradient gradient(baseColour.brighter(0.05f), 0.0f, 0.0f,
                                baseColour.darker(0.05f), 0.0f, bounds.getHeight(), false);
    g.setGradientFill(gradient);
    g.fillRoundedRectangle(bounds, cornerSize);
    
    // Add subtle border
    g.setColour(button.findColour(button.getToggleState() ? juce::TextButton::textColourOnId 
                                                        : juce::TextButton::textColourOffId)
                .withAlpha(0.4f));
    g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
    
    // Add illumination effect for active buttons
    if (button.getToggleState())
    {
        g.setColour(primaryColour.withAlpha(0.15f));
        g.fillRoundedRectangle(bounds, cornerSize);
    }
}

void ForensEQTheme::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                   float sliderPosProportional, float rotaryStartAngle,
                                   float rotaryEndAngle, juce::Slider& slider)
{
    // Calculate dimensions
    const float radius = juce::jmin(width / 2, height / 2) - 4.0f;
    const float centerX = x + width * 0.5f;
    const float centerY = y + height * 0.5f;
    const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    
    // Draw outer circle
    g.setColour(backgroundAccentColour);
    g.fillEllipse(centerX - radius, centerY - radius, radius * 2.0f, radius * 2.0f);
    
    // Draw track
    g.setColour(primaryColour.withAlpha(0.2f));
    juce::Path track;
    track.addArc(centerX - radius, centerY - radius, radius * 2.0f, radius * 2.0f,
                rotaryStartAngle, rotaryEndAngle, true);
    g.strokePath(track, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    // Draw filled arc
    g.setColour(primaryColour);
    juce::Path filledArc;
    filledArc.addArc(centerX - radius, centerY - radius, radius * 2.0f, radius * 2.0f,
                    rotaryStartAngle, angle, true);
    g.strokePath(filledArc, juce::PathStrokeType(3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    // Draw pointer
    juce::Path pointer;
    const float pointerLength = radius * 0.7f;
    const float pointerThickness = 2.0f;
    
    pointer.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    pointer.applyTransform(juce::AffineTransform::rotation(angle).translated(centerX, centerY));
    
    g.setColour(textHighlightColour);
    g.fillPath(pointer);
    
    // Add subtle glow for pointer
    if (slider.isEnabled())
    {
        g.setColour(primaryColour.withAlpha(0.3f));
        g.fillPath(pointer);
    }
}

void ForensEQTheme::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                   float sliderPos, float minSliderPos, float maxSliderPos,
                                   const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    // Use default implementation for now, can be customized later
    LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
}

void ForensEQTheme::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                   bool shouldDrawButtonAsHighlighted, 
                                   bool shouldDrawButtonAsDown)
{
    // Use default implementation for now, can be customized later
    LookAndFeel_V4::drawToggleButton(g, button, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
}

void ForensEQTheme::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                               int buttonX, int buttonY, int buttonW, int buttonH,
                               juce::ComboBox& box)
{
    // Use default implementation for now, can be customized later
    LookAndFeel_V4::drawComboBox(g, width, height, isButtonDown, buttonX, buttonY, buttonW, buttonH, box);
}

void ForensEQTheme::drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
                                    bool isSeparator, bool isActive, bool isHighlighted,
                                    bool isTicked, bool hasSubMenu, const juce::String& text,
                                    const juce::String& shortcutKeyText, const juce::Drawable* icon,
                                    const juce::Colour* textColour)
{
    // Use default implementation for now, can be customized later
    LookAndFeel_V4::drawPopupMenuItem(g, area, isSeparator, isActive, isHighlighted, isTicked, 
                                    hasSubMenu, text, shortcutKeyText, icon, textColour);
}

juce::Font ForensEQTheme::getTextButtonFont(juce::TextButton&, int buttonHeight)
{
    return juce::Font("Helvetica", buttonHeight * 0.6f, juce::Font::plain);
}
