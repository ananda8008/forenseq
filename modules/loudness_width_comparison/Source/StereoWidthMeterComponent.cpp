#include "StereoWidthMeterComponent.h"

namespace ForensEQ {

StereoWidthMeterComponent::StereoWidthMeterComponent()
{
    // Start the timer for animations
    startTimerHz(30);
}

StereoWidthMeterComponent::~StereoWidthMeterComponent()
{
    stopTimer();
}

void StereoWidthMeterComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    // Fill background
    g.fillAll(backgroundColor);
    
    // Draw meter background
    auto meterBounds = bounds.reduced(10, 20);
    g.setColour(meterBackgroundColor);
    g.fillRect(meterBounds);
    
    // Draw meter title
    g.setColour(textColor);
    g.setFont(14.0f);
    g.drawText(getWidthTypeString(), bounds.getX(), bounds.getY() + 5, bounds.getWidth(), 20, juce::Justification::centred);
    
    // Draw stereo field labels
    g.setFont(12.0f);
    g.drawText("Mono", meterBounds.getX(), meterBounds.getBottom() + 5, 40, 20, juce::Justification::left);
    g.drawText("Wide", meterBounds.getRight() - 40, meterBounds.getBottom() + 5, 40, 20, juce::Justification::right);
    
    // Calculate positions
    float refX = widthToX(displayReferenceWidth, meterBounds.getWidth());
    float userX = widthToX(displayUserWidth, meterBounds.getWidth());
    
    // Draw reference width marker
    int markerHeight = meterBounds.getHeight();
    int markerWidth = 4;
    
    juce::Rectangle<int> refMarkerBounds(
        meterBounds.getX() + refX - markerWidth / 2,
        meterBounds.getY(),
        markerWidth,
        markerHeight
    );
    g.setColour(referenceColor);
    g.fillRect(refMarkerBounds);
    
    // Draw user width marker
    juce::Rectangle<int> userMarkerBounds(
        meterBounds.getX() + userX - markerWidth / 2,
        meterBounds.getY(),
        markerWidth,
        markerHeight
    );
    g.setColour(userColor);
    g.fillRect(userMarkerBounds);
    
    // Draw glow effect when markers are close
    float distance = std::abs(userX - refX);
    if (distance < 20)
    {
        // Calculate glow intensity based on proximity
        float intensity = 1.0f - (distance / 20.0f);
        intensity *= 0.5f + 0.5f * std::sin(glowPhase); // Pulsating effect
        
        // Draw glow between markers
        int glowX = std::min(refX, userX);
        int glowWidth = std::abs(refX - userX) + markerWidth;
        juce::Rectangle<int> glowBounds(
            meterBounds.getX() + glowX - markerWidth / 2,
            meterBounds.getY(),
            glowWidth,
            markerHeight
        );
        
        drawGlowEffect(g, glowBounds, intensity);
    }
    
    // Draw reference and user labels
    g.setFont(12.0f);
    g.setColour(referenceColor);
    g.drawText("Reference", meterBounds.getX(), meterBounds.getY() - 20, meterBounds.getWidth() / 2, 20, juce::Justification::left);
    g.setColour(userColor);
    g.drawText("User", meterBounds.getX() + meterBounds.getWidth() / 2, meterBounds.getY() - 20, meterBounds.getWidth() / 2, 20, juce::Justification::right);
    
    // Draw difference text
    if (widthAnalyzer != nullptr)
    {
        float difference = userWidth - referenceWidth;
        juce::String diffText = widthAnalyzer->getWidthDifferenceDescription(difference, widthType);
        juce::Colour diffColor = widthAnalyzer->getWidthDifferenceColor(difference, widthType);
        
        g.setColour(diffColor);
        g.setFont(14.0f);
        g.drawText(diffText, bounds.getX(), bounds.getBottom() - 25, bounds.getWidth(), 20, juce::Justification::centred);
    }
}

void StereoWidthMeterComponent::resized()
{
    // Nothing specific needed here
}

void StereoWidthMeterComponent::setWidthValues(float userWidth, float referenceWidth, 
                                             StereoWidthAnalyzer::WidthType type)
{
    this->userWidth = userWidth;
    this->referenceWidth = referenceWidth;
    this->widthType = type;
    
    // Trigger a repaint
    repaint();
}

void StereoWidthMeterComponent::setStemType(ComparisonResult::StemType type)
{
    this->stemType = type;
    repaint();
}

void StereoWidthMeterComponent::setWidthAnalyzer(StereoWidthAnalyzer* analyzer)
{
    this->widthAnalyzer = analyzer;
}

void StereoWidthMeterComponent::timerCallback()
{
    // Animate the meters for smooth transitions
    const float smoothingFactor = 0.2f;
    
    displayUserWidth = displayUserWidth * (1.0f - smoothingFactor) + userWidth * smoothingFactor;
    displayReferenceWidth = displayReferenceWidth * (1.0f - smoothingFactor) + referenceWidth * smoothingFactor;
    
    // Update glow phase for animation
    glowPhase += 0.1f;
    if (glowPhase > juce::MathConstants<float>::twoPi)
        glowPhase -= juce::MathConstants<float>::twoPi;
    
    // Only repaint if there's a significant change or glow animation is active
    float distance = std::abs(widthToX(displayUserWidth, getWidth()) - widthToX(displayReferenceWidth, getWidth()));
    if (std::abs(displayUserWidth - userWidth) > 0.01f || 
        std::abs(displayReferenceWidth - referenceWidth) > 0.01f ||
        distance < 20)
    {
        repaint();
    }
}

float StereoWidthMeterComponent::widthToX(float width, float meterWidth) const
{
    // Convert width to a normalized value (0.0 to 1.0) based on width type
    float normalized;
    
    switch (widthType)
    {
        case StereoWidthAnalyzer::WidthType::Correlation:
            // Correlation: -1.0 (wide) to 1.0 (mono)
            normalized = (1.0f - width) * 0.5f;
            break;
            
        case StereoWidthAnalyzer::WidthType::MidSideRatio:
            // Mid/Side ratio: 0.0 (mono) to higher values (wide)
            // Use a logarithmic scale for better visualization
            normalized = 0.5f * (1.0f + std::tanh(width - 1.0f));
            break;
            
        case StereoWidthAnalyzer::WidthType::Percentage:
            // Percentage: 0% (mono) to 100% (wide)
            normalized = width / 100.0f;
            break;
            
        default:
            normalized = 0.5f;
    }
    
    // Clamp to valid range
    normalized = juce::jlimit(0.0f, 1.0f, normalized);
    
    // Convert to X coordinate
    return normalized * meterWidth;
}

juce::String StereoWidthMeterComponent::getWidthTypeString() const
{
    switch (widthType)
    {
        case StereoWidthAnalyzer::WidthType::Correlation:
            return "Stereo Correlation";
        case StereoWidthAnalyzer::WidthType::MidSideRatio:
            return "Mid/Side Ratio";
        case StereoWidthAnalyzer::WidthType::Percentage:
            return "Stereo Width";
        default:
            return "Width";
    }
}

void StereoWidthMeterComponent::drawGlowEffect(juce::Graphics& g, juce::Rectangle<int> bounds, float intensity)
{
    // Create a radial gradient for the glow effect
    juce::ColourGradient gradient;
    
    // Center of the glow
    float centerX = bounds.getCentreX();
    float centerY = bounds.getCentreY();
    
    // Radius of the glow
    float radius = bounds.getWidth() * 0.5f;
    
    // Create gradient from center (white) to edges (transparent)
    gradient = juce::ColourGradient(
        glowColor.withAlpha(intensity),
        centerX, centerY,
        glowColor.withAlpha(0.0f),
        centerX + radius, centerY,
        true
    );
    
    // Fill with gradient
    g.setGradientFill(gradient);
    g.fillRect(bounds);
}

} // namespace ForensEQ
