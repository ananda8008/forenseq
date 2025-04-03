#include "LoudnessWidthComparisonComponent.h"

namespace ForensEQ {

LoudnessWidthComparisonComponent::LoudnessWidthComparisonComponent()
{
    // Add all child components
    addAndMakeVisible(integratedLUFSMeter);
    addAndMakeVisible(shortTermLUFSMeter);
    addAndMakeVisible(momentaryLUFSMeter);
    addAndMakeVisible(rmsMeter);
    addAndMakeVisible(widthMeter);
    addAndMakeVisible(matchScoreDisplay);
    
    // Set initial loudness meter types
    integratedLUFSMeter.setLoudnessValues(-23.0f, -23.0f, LoudnessAnalyzer::LoudnessType::Integrated);
    shortTermLUFSMeter.setLoudnessValues(-23.0f, -23.0f, LoudnessAnalyzer::LoudnessType::ShortTerm);
    momentaryLUFSMeter.setLoudnessValues(-23.0f, -23.0f, LoudnessAnalyzer::LoudnessType::Momentary);
    rmsMeter.setLoudnessValues(-23.0f, -23.0f, LoudnessAnalyzer::LoudnessType::RMS);
    
    // Set initial width meter type
    widthMeter.setWidthValues(50.0f, 50.0f, StereoWidthAnalyzer::WidthType::Percentage);
    
    // Set initial match scores
    matchScoreDisplay.setMatchScores(1.0f, 1.0f, 1.0f);
    
    // Set initial stem type for all components
    setStemType(ComparisonResult::StemType::FullMix);
}

LoudnessWidthComparisonComponent::~LoudnessWidthComparisonComponent()
{
}

void LoudnessWidthComparisonComponent::paint(juce::Graphics& g)
{
    // Fill background
    g.fillAll(backgroundColor);
    
    // Draw title
    g.setColour(textColor);
    g.setFont(18.0f);
    g.drawText("Loudness & Width Comparison", 0, 10, getWidth(), 30, juce::Justification::centred);
}

void LoudnessWidthComparisonComponent::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(50); // Space for title
    
    // Calculate component sizes
    int meterHeight = bounds.getHeight() / 3;
    int meterWidth = bounds.getWidth() / 2 - 10;
    
    // Layout loudness meters (top row)
    integratedLUFSMeter.setBounds(bounds.getX(), bounds.getY(), meterWidth, meterHeight);
    shortTermLUFSMeter.setBounds(bounds.getX() + meterWidth + 10, bounds.getY(), meterWidth, meterHeight);
    
    // Layout more loudness meters (middle row)
    momentaryLUFSMeter.setBounds(bounds.getX(), bounds.getY() + meterHeight + 10, meterWidth, meterHeight);
    rmsMeter.setBounds(bounds.getX() + meterWidth + 10, bounds.getY() + meterHeight + 10, meterWidth, meterHeight);
    
    // Layout width meter and match score (bottom row)
    widthMeter.setBounds(bounds.getX(), bounds.getY() + 2 * (meterHeight + 10), meterWidth, meterHeight);
    matchScoreDisplay.setBounds(bounds.getX() + meterWidth + 10, bounds.getY() + 2 * (meterHeight + 10), meterWidth, meterHeight);
}

void LoudnessWidthComparisonComponent::setComparisonResult(const ComparisonResult& result)
{
    currentResult = result;
    
    // Update all components with the new data for the current stem type
    updateComponentsForCurrentStem();
}

void LoudnessWidthComparisonComponent::setStemType(ComparisonResult::StemType type)
{
    currentStemType = type;
    
    // Update all component stem types
    integratedLUFSMeter.setStemType(type);
    shortTermLUFSMeter.setStemType(type);
    momentaryLUFSMeter.setStemType(type);
    rmsMeter.setStemType(type);
    widthMeter.setStemType(type);
    matchScoreDisplay.setStemType(type);
    
    // Update all components with the data for the new stem type
    updateComponentsForCurrentStem();
}

void LoudnessWidthComparisonComponent::setAnalyzers(LoudnessAnalyzer* loudnessAnalyzer, StereoWidthAnalyzer* widthAnalyzer)
{
    // Set analyzers for all components that need them
    integratedLUFSMeter.setLoudnessAnalyzer(loudnessAnalyzer);
    shortTermLUFSMeter.setLoudnessAnalyzer(loudnessAnalyzer);
    momentaryLUFSMeter.setLoudnessAnalyzer(loudnessAnalyzer);
    rmsMeter.setLoudnessAnalyzer(loudnessAnalyzer);
    widthMeter.setWidthAnalyzer(widthAnalyzer);
}

void LoudnessWidthComparisonComponent::updateComponentsForCurrentStem()
{
    // Update loudness meters
    integratedLUFSMeter.setLoudnessValues(
        currentResult.getUserLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::Integrated),
        currentResult.getReferenceLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::Integrated),
        LoudnessAnalyzer::LoudnessType::Integrated
    );
    
    shortTermLUFSMeter.setLoudnessValues(
        currentResult.getUserLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::ShortTerm),
        currentResult.getReferenceLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::ShortTerm),
        LoudnessAnalyzer::LoudnessType::ShortTerm
    );
    
    momentaryLUFSMeter.setLoudnessValues(
        currentResult.getUserLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::Momentary),
        currentResult.getReferenceLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::Momentary),
        LoudnessAnalyzer::LoudnessType::Momentary
    );
    
    rmsMeter.setLoudnessValues(
        currentResult.getUserLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::RMS),
        currentResult.getReferenceLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::RMS),
        LoudnessAnalyzer::LoudnessType::RMS
    );
    
    // Update width meter
    widthMeter.setWidthValues(
        currentResult.getUserWidth(currentStemType, StereoWidthAnalyzer::WidthType::Percentage),
        currentResult.getReferenceWidth(currentStemType, StereoWidthAnalyzer::WidthType::Percentage),
        StereoWidthAnalyzer::WidthType::Percentage
    );
    
    // Update match score display
    matchScoreDisplay.setMatchScores(
        currentResult.getLoudnessMatchScore(currentStemType, LoudnessAnalyzer::LoudnessType::Integrated),
        currentResult.getWidthMatchScore(currentStemType, StereoWidthAnalyzer::WidthType::Percentage),
        currentResult.getCombinedMatchScore(currentStemType)
    );
}

} // namespace ForensEQ
