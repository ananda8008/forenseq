#include "LightbulbToggleButton.h"

namespace ForensEQ {

LightbulbToggleButton::LightbulbToggleButton()
    : juce::Button("LightbulbToggleButton")
{
    // Set up tooltip
    setTooltip(inactiveTooltip);
    
    // Start the animation timer
    startTimer(50); // Update every 50ms
}

LightbulbToggleButton::~LightbulbToggleButton()
{
    stopTimer();
}

void LightbulbToggleButton::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    // Get the button state
    bool isOn = getToggleState();
    
    // Calculate the bounds for the bulb
    juce::Rectangle<float> bounds = getLocalBounds().toFloat().reduced(4.0f);
    
    // Calculate glow amount based on toggle state and animation
    float glowAmount = isOn ? 0.5f + glowIntensity * 0.5f : 0.0f;
    
    // Draw the components of the lightbulb
    drawBase(g, bounds, isOn);
    drawBulb(g, bounds, isOn, glowAmount);
    drawFilament(g, bounds, isOn, glowAmount);
    
    // Draw glow effect if the button is on
    if (isOn)
    {
        drawGlow(g, bounds, glowAmount);
    }
    
    // Draw highlight if needed
    if (shouldDrawButtonAsHighlighted)
    {
        g.setColour(juce::Colours::white.withAlpha(0.2f));
        g.drawRoundedRectangle(bounds, 8.0f, 1.0f);
    }
    
    // Draw pressed state if needed
    if (shouldDrawButtonAsDown)
    {
        g.setColour(juce::Colours::white.withAlpha(0.3f));
        g.drawRoundedRectangle(bounds.reduced(1.0f), 8.0f, 2.0f);
    }
}

void LightbulbToggleButton::timerCallback()
{
    // Update glow animation
    if (getToggleState())
    {
        // Animate the glow when the button is on
        glowPhase += 0.05f;
        
        if (glowPhase > juce::MathConstants<float>::twoPi)
            glowPhase -= juce::MathConstants<float>::twoPi;
        
        // Subtle pulsing effect
        glowIntensity = 0.7f + 0.3f * std::sin(glowPhase);
        
        repaint();
    }
    else if (glowIntensity > 0.0f)
    {
        // Fade out when turning off
        glowIntensity = juce::jmax(0.0f, glowIntensity - 0.1f);
        repaint();
    }
}

void LightbulbToggleButton::setTooltipText(const juce::String& activeText, const juce::String& inactiveText)
{
    activeTooltip = activeText;
    inactiveTooltip = inactiveText;
    
    // Update current tooltip based on state
    setTooltip(getToggleState() ? activeTooltip : inactiveTooltip);
}

void LightbulbToggleButton::drawBulb(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool isOn, float glowAmount)
{
    // Calculate bulb shape
    float bulbWidth = bounds.getWidth() * 0.7f;
    float bulbHeight = bounds.getHeight() * 0.6f;
    juce::Rectangle<float> bulbBounds(
        bounds.getCentreX() - bulbWidth / 2.0f,
        bounds.getY(),
        bulbWidth,
        bulbHeight
    );
    
    // Draw bulb glass
    juce::Path bulbPath;
    bulbPath.addEllipse(bulbBounds);
    
    // Glass color based on state
    if (isOn)
    {
        // Glowing bulb
        g.setGradientFill(juce::ColourGradient(
            juce::Colour(255, 220, 150).withAlpha(0.9f),
            bulbBounds.getCentre(),
            juce::Colour(255, 180, 80).withAlpha(0.6f),
            bulbBounds.getBottomLeft(),
            true
        ));
    }
    else
    {
        // Off bulb - slightly transparent glass
        g.setGradientFill(juce::ColourGradient(
            juce::Colour(200, 200, 200).withAlpha(0.3f),
            bulbBounds.getCentre(),
            juce::Colour(150, 150, 150).withAlpha(0.2f),
            bulbBounds.getBottomLeft(),
            true
        ));
    }
    
    g.fillPath(bulbPath);
    
    // Draw bulb outline
    g.setColour(juce::Colour(100, 100, 100).withAlpha(0.8f));
    g.strokePath(bulbPath, juce::PathStrokeType(1.0f));
}

void LightbulbToggleButton::drawFilament(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool isOn, float glowAmount)
{
    // Calculate filament position
    float bulbWidth = bounds.getWidth() * 0.7f;
    float bulbHeight = bounds.getHeight() * 0.6f;
    juce::Rectangle<float> bulbBounds(
        bounds.getCentreX() - bulbWidth / 2.0f,
        bounds.getY(),
        bulbWidth,
        bulbHeight
    );
    
    // Draw filament
    juce::Path filamentPath;
    
    // Start at the bottom center
    filamentPath.startNewSubPath(bulbBounds.getCentreX(), bulbBounds.getBottom() - 5.0f);
    
    // Create a zigzag pattern for the filament
    float zigzagWidth = bulbWidth * 0.3f;
    float zigzagHeight = bulbHeight * 0.6f;
    float zigzagY = bulbBounds.getCentreY() - zigzagHeight / 2.0f;
    
    filamentPath.lineTo(bulbBounds.getCentreX(), zigzagY + zigzagHeight);
    filamentPath.lineTo(bulbBounds.getCentreX() - zigzagWidth / 2.0f, zigzagY + zigzagHeight * 0.75f);
    filamentPath.lineTo(bulbBounds.getCentreX() + zigzagWidth / 2.0f, zigzagY + zigzagHeight * 0.5f);
    filamentPath.lineTo(bulbBounds.getCentreX() - zigzagWidth / 2.0f, zigzagY + zigzagHeight * 0.25f);
    filamentPath.lineTo(bulbBounds.getCentreX() + zigzagWidth / 2.0f, zigzagY);
    filamentPath.lineTo(bulbBounds.getCentreX(), zigzagY - 5.0f);
    
    // Filament color based on state
    if (isOn)
    {
        // Glowing filament
        g.setColour(juce::Colour(255, 180, 50).withAlpha(0.9f + glowAmount * 0.1f));
        g.strokePath(filamentPath, juce::PathStrokeType(2.0f + glowAmount * 1.0f));
        
        // Add inner glow
        g.setColour(juce::Colour(255, 220, 150).withAlpha(0.7f * glowAmount));
        g.strokePath(filamentPath, juce::PathStrokeType(4.0f * glowAmount));
    }
    else
    {
        // Off filament
        g.setColour(juce::Colour(100, 100, 100).withAlpha(0.8f));
        g.strokePath(filamentPath, juce::PathStrokeType(1.5f));
    }
}

void LightbulbToggleButton::drawBase(juce::Graphics& g, const juce::Rectangle<float>& bounds, bool isOn)
{
    // Calculate base position
    float baseWidth = bounds.getWidth() * 0.4f;
    float baseHeight = bounds.getHeight() * 0.3f;
    float bulbHeight = bounds.getHeight() * 0.6f;
    
    juce::Rectangle<float> baseBounds(
        bounds.getCentreX() - baseWidth / 2.0f,
        bounds.getY() + bulbHeight - 5.0f,
        baseWidth,
        baseHeight
    );
    
    // Draw base
    juce::Path basePath;
    basePath.addRoundedRectangle(baseBounds, 2.0f);
    
    // Base gradient
    g.setGradientFill(juce::ColourGradient(
        juce::Colour(80, 80, 80),
        baseBounds.getTopLeft(),
        juce::Colour(40, 40, 40),
        baseBounds.getBottomRight(),
        true
    ));
    
    g.fillPath(basePath);
    
    // Draw base outline
    g.setColour(juce::Colour(30, 30, 30));
    g.strokePath(basePath, juce::PathStrokeType(1.0f));
    
    // Draw base details (screw threads)
    float threadSpacing = baseHeight / 5.0f;
    g.setColour(juce::Colour(60, 60, 60));
    
    for (int i = 1; i < 5; ++i)
    {
        float y = baseBounds.getY() + i * threadSpacing;
        g.drawLine(baseBounds.getX() + 2.0f, y, baseBounds.getRight() - 2.0f, y, 1.0f);
    }
}

void LightbulbToggleButton::drawGlow(juce::Graphics& g, const juce::Rectangle<float>& bounds, float intensity)
{
    // Calculate bulb position for glow
    float bulbWidth = bounds.getWidth() * 0.7f;
    float bulbHeight = bounds.getHeight() * 0.6f;
    juce::Rectangle<float> bulbBounds(
        bounds.getCentreX() - bulbWidth / 2.0f,
        bounds.getY(),
        bulbWidth,
        bulbHeight
    );
    
    // Create a larger ellipse for the glow
    float glowSize = intensity * 20.0f;
    juce::Rectangle<float> glowBounds = bulbBounds.expanded(glowSize);
    
    // Draw glow
    juce::ColourGradient gradient(
        juce::Colour(255, 220, 100).withAlpha(0.7f * intensity),
        bulbBounds.getCentre(),
        juce::Colours::transparentWhite,
        glowBounds.getTopLeft(),
        true
    );
    
    g.setGradientFill(gradient);
    g.fillEllipse(glowBounds);
}

} // namespace ForensEQ
