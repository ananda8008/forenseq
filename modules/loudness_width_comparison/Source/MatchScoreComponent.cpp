#include "MatchScoreComponent.h"

namespace ForensEQ {

MatchScoreComponent::MatchScoreComponent()
{
}

MatchScoreComponent::~MatchScoreComponent()
{
}

void MatchScoreComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    // Fill background
    g.fillAll(backgroundColor);
    
    // Draw title
    g.setColour(textColor);
    g.setFont(16.0f);
    juce::String title = getStemTypeString() + " Match Score";
    g.drawText(title, bounds.getX(), bounds.getY() + 5, bounds.getWidth(), 25, juce::Justification::centred);
    
    // Draw score circles
    int circleSize = juce::jmin(bounds.getWidth() / 4, bounds.getHeight() / 2);
    int circleY = bounds.getCentreY();
    int padding = circleSize / 4;
    
    // Loudness score circle
    int loudnessX = bounds.getWidth() / 4;
    juce::Rectangle<int> loudnessCircleBounds(
        loudnessX - circleSize / 2,
        circleY - circleSize / 2,
        circleSize,
        circleSize
    );
    
    // Width score circle
    int widthX = bounds.getWidth() / 2;
    juce::Rectangle<int> widthCircleBounds(
        widthX - circleSize / 2,
        circleY - circleSize / 2,
        circleSize,
        circleSize
    );
    
    // Combined score circle
    int combinedX = 3 * bounds.getWidth() / 4;
    juce::Rectangle<int> combinedCircleBounds(
        combinedX - circleSize / 2,
        circleY - circleSize / 2,
        circleSize,
        circleSize
    );
    
    // Draw score backgrounds
    g.setColour(loudnessColor.withAlpha(0.3f));
    g.fillEllipse(loudnessCircleBounds.toFloat());
    
    g.setColour(widthColor.withAlpha(0.3f));
    g.fillEllipse(widthCircleBounds.toFloat());
    
    g.setColour(combinedColor.withAlpha(0.3f));
    g.fillEllipse(combinedCircleBounds.toFloat());
    
    // Draw score fills based on score values
    g.setColour(loudnessColor);
    g.fillEllipse(loudnessCircleBounds.toFloat().withSizeKeepingCentre(
        circleSize * loudnessScore,
        circleSize * loudnessScore
    ));
    
    g.setColour(widthColor);
    g.fillEllipse(widthCircleBounds.toFloat().withSizeKeepingCentre(
        circleSize * widthScore,
        circleSize * widthScore
    ));
    
    g.setColour(combinedColor);
    g.fillEllipse(combinedCircleBounds.toFloat().withSizeKeepingCentre(
        circleSize * combinedScore,
        circleSize * combinedScore
    ));
    
    // Draw score outlines
    g.setColour(loudnessColor);
    g.drawEllipse(loudnessCircleBounds.toFloat(), 2.0f);
    
    g.setColour(widthColor);
    g.drawEllipse(widthCircleBounds.toFloat(), 2.0f);
    
    g.setColour(combinedColor);
    g.drawEllipse(combinedCircleBounds.toFloat(), 2.0f);
    
    // Draw score values
    g.setFont(16.0f);
    
    g.setColour(loudnessColor);
    g.drawText(juce::String(int(loudnessScore * 100)) + "%", 
               loudnessCircleBounds.getX(), 
               loudnessCircleBounds.getCentreY() - 8, 
               loudnessCircleBounds.getWidth(), 
               16, 
               juce::Justification::centred);
    
    g.setColour(widthColor);
    g.drawText(juce::String(int(widthScore * 100)) + "%", 
               widthCircleBounds.getX(), 
               widthCircleBounds.getCentreY() - 8, 
               widthCircleBounds.getWidth(), 
               16, 
               juce::Justification::centred);
    
    g.setColour(combinedColor);
    g.drawText(juce::String(int(combinedScore * 100)) + "%", 
               combinedCircleBounds.getX(), 
               combinedCircleBounds.getCentreY() - 8, 
               combinedCircleBounds.getWidth(), 
               16, 
               juce::Justification::centred);
    
    // Draw score labels
    g.setFont(14.0f);
    
    g.setColour(loudnessColor);
    g.drawText("Loudness", 
               loudnessCircleBounds.getX(), 
               loudnessCircleBounds.getBottom() + padding, 
               loudnessCircleBounds.getWidth(), 
               20, 
               juce::Justification::centred);
    
    g.setColour(widthColor);
    g.drawText("Width", 
               widthCircleBounds.getX(), 
               widthCircleBounds.getBottom() + padding, 
               widthCircleBounds.getWidth(), 
               20, 
               juce::Justification::centred);
    
    g.setColour(combinedColor);
    g.drawText("Overall", 
               combinedCircleBounds.getX(), 
               combinedCircleBounds.getBottom() + padding, 
               combinedCircleBounds.getWidth(), 
               20, 
               juce::Justification::centred);
}

void MatchScoreComponent::resized()
{
    // Nothing specific needed here
}

void MatchScoreComponent::setMatchScores(float loudnessScore, float widthScore, float combinedScore)
{
    this->loudnessScore = loudnessScore;
    this->widthScore = widthScore;
    this->combinedScore = combinedScore;
    
    // Trigger a repaint
    repaint();
}

void MatchScoreComponent::setStemType(ComparisonResult::StemType type)
{
    this->stemType = type;
    repaint();
}

juce::String MatchScoreComponent::getStemTypeString() const
{
    switch (stemType)
    {
        case ComparisonResult::StemType::FullMix:
            return "Full Mix";
        case ComparisonResult::StemType::Kick:
            return "Kick";
        case ComparisonResult::StemType::Snare:
            return "Snare";
        case ComparisonResult::StemType::Bass:
            return "Bass";
        case ComparisonResult::StemType::Vocals:
            return "Vocals";
        case ComparisonResult::StemType::Other:
            return "Other";
        default:
            return "Unknown";
    }
}

juce::Colour MatchScoreComponent::getScoreColor(float score) const
{
    // Red for low scores, yellow for medium, green for high
    if (score < 0.5f)
    {
        float t = score / 0.5f;
        return juce::Colour::fromRGB(
            255,
            static_cast<uint8_t>(t * 255),
            0
        );
    }
    else
    {
        float t = (score - 0.5f) / 0.5f;
        return juce::Colour::fromRGB(
            static_cast<uint8_t>((1.0f - t) * 255),
            255,
            0
        );
    }
}

} // namespace ForensEQ
