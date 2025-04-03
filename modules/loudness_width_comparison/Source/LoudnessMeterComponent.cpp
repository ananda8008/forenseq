#include "LoudnessMeterComponent.h"

namespace ForensEQ {

LoudnessMeterComponent::LoudnessMeterComponent()
{
    // Start the timer for animations
    startTimerHz(30);
}

LoudnessMeterComponent::~LoudnessMeterComponent()
{
    stopTimer();
}

void LoudnessMeterComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    // Fill background
    g.fillAll(backgroundColor);
    
    // Draw meter background
    auto meterBounds = bounds.reduced(10);
    g.setColour(meterBackgroundColor);
    g.fillRect(meterBounds);
    
    // Draw reference loudness meter
    float refHeight = dbToY(displayReferenceLoudness, meterBounds.getHeight());
    juce::Rectangle<int> refMeterBounds(
        meterBounds.getX(),
        meterBounds.getY() + (meterBounds.getHeight() - refHeight),
        meterBounds.getWidth() / 2 - 2,
        refHeight
    );
    g.setColour(referenceColor);
    g.fillRect(refMeterBounds);
    
    // Draw user loudness meter
    float userHeight = dbToY(displayUserLoudness, meterBounds.getHeight());
    juce::Rectangle<int> userMeterBounds(
        meterBounds.getX() + meterBounds.getWidth() / 2 + 2,
        meterBounds.getY() + (meterBounds.getHeight() - userHeight),
        meterBounds.getWidth() / 2 - 2,
        userHeight
    );
    g.setColour(userColor);
    g.fillRect(userMeterBounds);
    
    // Draw meter labels
    g.setColour(textColor);
    g.setFont(12.0f);
    
    // Draw dB scale
    for (int db = -60; db <= 0; db += 10)
    {
        float y = meterBounds.getY() + meterBounds.getHeight() - dbToY(db, meterBounds.getHeight());
        g.drawText(juce::String(db), meterBounds.getX() - 5, y - 6, 20, 12, juce::Justification::right);
        g.drawLine(meterBounds.getX(), y, meterBounds.getRight(), y, 0.5f);
    }
    
    // Draw meter title
    g.setFont(14.0f);
    g.drawText(getLoudnessTypeString(), bounds.getX(), bounds.getY() + 5, bounds.getWidth(), 20, juce::Justification::centred);
    
    // Draw reference and user labels
    g.setFont(12.0f);
    g.setColour(referenceColor);
    g.drawText("Reference", meterBounds.getX(), meterBounds.getBottom() + 5, meterBounds.getWidth() / 2 - 2, 20, juce::Justification::centred);
    g.setColour(userColor);
    g.drawText("User", meterBounds.getX() + meterBounds.getWidth() / 2 + 2, meterBounds.getBottom() + 5, meterBounds.getWidth() / 2 - 2, 20, juce::Justification::centred);
    
    // Draw difference text
    if (loudnessAnalyzer != nullptr)
    {
        float difference = userLoudness - referenceLoudness;
        juce::String diffText = loudnessAnalyzer->getLoudnessDifferenceDescription(difference, loudnessType);
        juce::Colour diffColor = loudnessAnalyzer->getLoudnessDifferenceColor(difference, loudnessType);
        
        g.setColour(diffColor);
        g.setFont(14.0f);
        g.drawText(diffText, bounds.getX(), bounds.getBottom() - 25, bounds.getWidth(), 20, juce::Justification::centred);
    }
}

void LoudnessMeterComponent::resized()
{
    // Nothing specific needed here
}

void LoudnessMeterComponent::setLoudnessValues(float userLoudness, float referenceLoudness, 
                                             LoudnessAnalyzer::LoudnessType type)
{
    this->userLoudness = userLoudness;
    this->referenceLoudness = referenceLoudness;
    this->loudnessType = type;
    
    // Trigger a repaint
    repaint();
}

void LoudnessMeterComponent::setStemType(ComparisonResult::StemType type)
{
    this->stemType = type;
    repaint();
}

void LoudnessMeterComponent::setLoudnessAnalyzer(LoudnessAnalyzer* analyzer)
{
    this->loudnessAnalyzer = analyzer;
}

void LoudnessMeterComponent::timerCallback()
{
    // Animate the meters for smooth transitions
    const float smoothingFactor = 0.2f;
    
    displayUserLoudness = displayUserLoudness * (1.0f - smoothingFactor) + userLoudness * smoothingFactor;
    displayReferenceLoudness = displayReferenceLoudness * (1.0f - smoothingFactor) + referenceLoudness * smoothingFactor;
    
    // Only repaint if there's a significant change
    if (std::abs(displayUserLoudness - userLoudness) > 0.01f || 
        std::abs(displayReferenceLoudness - referenceLoudness) > 0.01f)
    {
        repaint();
    }
}

float LoudnessMeterComponent::dbToY(float db, float height) const
{
    // Clamp the dB value to the meter range
    db = juce::jlimit(meterMinDb, meterMaxDb, db);
    
    // Convert dB to a normalized value (0.0 to 1.0)
    float normalized = (db - meterMinDb) / (meterMaxDb - meterMinDb);
    
    // Convert to Y coordinate (0 = top, height = bottom)
    return normalized * height;
}

juce::String LoudnessMeterComponent::getLoudnessTypeString() const
{
    switch (loudnessType)
    {
        case LoudnessAnalyzer::LoudnessType::Integrated:
            return "Integrated LUFS";
        case LoudnessAnalyzer::LoudnessType::ShortTerm:
            return "Short-Term LUFS";
        case LoudnessAnalyzer::LoudnessType::Momentary:
            return "Momentary LUFS";
        case LoudnessAnalyzer::LoudnessType::RMS:
            return "RMS Level";
        default:
            return "Loudness";
    }
}

} // namespace ForensEQ
