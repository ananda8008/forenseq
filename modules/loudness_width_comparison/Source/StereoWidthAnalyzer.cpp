#include "StereoWidthAnalyzer.h"

namespace ForensEQ {

StereoWidthAnalyzer::StereoWidthAnalyzer()
{
}

StereoWidthAnalyzer::~StereoWidthAnalyzer()
{
}

float StereoWidthAnalyzer::calculateWidth(const juce::AudioBuffer<float>& buffer, WidthType type)
{
    switch (type)
    {
        case WidthType::Correlation:
            return calculateCorrelation(buffer);
        case WidthType::MidSideRatio:
            return calculateMidSideRatio(buffer);
        case WidthType::Percentage:
            // Convert correlation to percentage for a more intuitive measure
            return correlationToPercentage(calculateCorrelation(buffer));
        default:
            return 0.0f;
    }
}

float StereoWidthAnalyzer::calculateWidthDifference(const juce::AudioBuffer<float>& userBuffer,
                                                  const juce::AudioBuffer<float>& referenceBuffer,
                                                  WidthType type)
{
    float userWidth = calculateWidth(userBuffer, type);
    float referenceWidth = calculateWidth(referenceBuffer, type);
    
    // Return the difference (positive means user is wider than reference)
    return userWidth - referenceWidth;
}

juce::String StereoWidthAnalyzer::getWidthDifferenceDescription(float difference, WidthType type)
{
    // Adjust description based on width type
    switch (type)
    {
        case WidthType::Correlation:
            if (std::abs(difference) < 0.05f)
                return "Matched";
            else if (difference > 0)
                return "+" + juce::String(difference, 2) + " wider";
            else
                return juce::String(difference, 2) + " narrower";
            
        case WidthType::MidSideRatio:
            if (std::abs(difference) < 0.1f)
                return "Matched";
            else if (difference > 0)
                return "+" + juce::String(difference, 2) + " more side";
            else
                return juce::String(difference, 2) + " less side";
            
        case WidthType::Percentage:
            if (std::abs(difference) < 5.0f)
                return "Matched";
            else if (difference > 0)
                return "+" + juce::String(difference, 1) + "% wider";
            else
                return juce::String(difference, 1) + "% narrower";
            
        default:
            return "Unknown";
    }
}

juce::Colour StereoWidthAnalyzer::getWidthDifferenceColor(float difference, WidthType type)
{
    // Normalize difference based on width type
    float normalizedDifference;
    
    switch (type)
    {
        case WidthType::Correlation:
            normalizedDifference = std::abs(difference);
            break;
        case WidthType::MidSideRatio:
            normalizedDifference = std::abs(difference) / 0.5f; // Scale for comparison
            break;
        case WidthType::Percentage:
            normalizedDifference = std::abs(difference) / 100.0f;
            break;
        default:
            normalizedDifference = 0.0f;
    }
    
    // Perfect match (green)
    if (normalizedDifference < perfectMatchThreshold)
    {
        return juce::Colour(0, 255, 0);
    }
    // Good match (yellow-green)
    else if (normalizedDifference < goodMatchThreshold)
    {
        float t = (normalizedDifference - perfectMatchThreshold) / (goodMatchThreshold - perfectMatchThreshold);
        return juce::Colour(
            static_cast<uint8_t>(t * 255),  // R: 0 -> 255
            static_cast<uint8_t>(255),      // G: 255 -> 255
            static_cast<uint8_t>(0)         // B: 0 -> 0
        );
    }
    // Fair match (yellow to orange)
    else if (normalizedDifference < fairMatchThreshold)
    {
        float t = (normalizedDifference - goodMatchThreshold) / (fairMatchThreshold - goodMatchThreshold);
        return juce::Colour(
            static_cast<uint8_t>(255),                  // R: 255 -> 255
            static_cast<uint8_t>(255 * (1.0f - t)),     // G: 255 -> 0
            static_cast<uint8_t>(0)                     // B: 0 -> 0
        );
    }
    // Poor match (red)
    else
    {
        return juce::Colour(255, 0, 0);
    }
}

float StereoWidthAnalyzer::calculateWidthMatchScore(const juce::AudioBuffer<float>& userBuffer,
                                                  const juce::AudioBuffer<float>& referenceBuffer,
                                                  WidthType type)
{
    // Normalize difference based on width type
    float difference;
    
    switch (type)
    {
        case WidthType::Correlation:
            difference = std::abs(calculateWidthDifference(userBuffer, referenceBuffer, type));
            break;
        case WidthType::MidSideRatio:
            difference = std::abs(calculateWidthDifference(userBuffer, referenceBuffer, type)) / 0.5f;
            break;
        case WidthType::Percentage:
            difference = std::abs(calculateWidthDifference(userBuffer, referenceBuffer, type)) / 100.0f;
            break;
        default:
            difference = 1.0f; // Maximum difference
    }
    
    // Perfect match
    if (difference < perfectMatchThreshold)
    {
        return 1.0f;
    }
    // Good match
    else if (difference < goodMatchThreshold)
    {
        float t = (difference - perfectMatchThreshold) / (goodMatchThreshold - perfectMatchThreshold);
        return 1.0f - (0.25f * t); // 1.0 -> 0.75
    }
    // Fair match
    else if (difference < fairMatchThreshold)
    {
        float t = (difference - goodMatchThreshold) / (fairMatchThreshold - goodMatchThreshold);
        return 0.75f - (0.25f * t); // 0.75 -> 0.5
    }
    // Poor match
    else
    {
        // Exponential falloff for very poor matches
        return 0.5f * std::exp(-(difference - fairMatchThreshold) / 0.2f);
    }
}

float StereoWidthAnalyzer::correlationToPercentage(float correlation)
{
    // Convert correlation coefficient (-1 to 1) to percentage (0 to 100)
    // -1 = out of phase (wide) = 100%
    // 0 = uncorrelated = 50%
    // 1 = in phase (mono) = 0%
    
    return (1.0f - correlation) * 50.0f;
}

float StereoWidthAnalyzer::midSideRatioToPercentage(float ratio)
{
    // Convert mid/side ratio to percentage (0 to 100)
    // 0 = no side (mono) = 0%
    // 1 = equal mid and side = 50%
    // >1 = more side than mid = >50%
    
    // Use a logarithmic scale for more intuitive mapping
    return 50.0f * (1.0f + std::tanh(ratio - 1.0f));
}

float StereoWidthAnalyzer::calculateCorrelation(const juce::AudioBuffer<float>& buffer)
{
    // Need at least stereo for correlation
    if (buffer.getNumChannels() < 2 || buffer.getNumSamples() == 0)
        return 1.0f; // Mono
    
    const float* leftData = buffer.getReadPointer(0);
    const float* rightData = buffer.getReadPointer(1);
    
    double sumLeft = 0.0;
    double sumRight = 0.0;
    double sumLeftSquared = 0.0;
    double sumRightSquared = 0.0;
    double sumLeftRight = 0.0;
    
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        float left = leftData[i];
        float right = rightData[i];
        
        sumLeft += left;
        sumRight += right;
        sumLeftSquared += left * left;
        sumRightSquared += right * right;
        sumLeftRight += left * right;
    }
    
    double n = static_cast<double>(buffer.getNumSamples());
    double numerator = n * sumLeftRight - sumLeft * sumRight;
    double denominator = std::sqrt((n * sumLeftSquared - sumLeft * sumLeft) * 
                                  (n * sumRightSquared - sumRight * sumRight));
    
    if (denominator < 1.0e-6)
        return 1.0f; // Avoid division by zero, assume mono
    
    // Correlation coefficient (-1 to 1)
    return static_cast<float>(numerator / denominator);
}

float StereoWidthAnalyzer::calculateMidSideRatio(const juce::AudioBuffer<float>& buffer)
{
    // Need at least stereo for mid/side
    if (buffer.getNumChannels() < 2 || buffer.getNumSamples() == 0)
        return 0.0f; // Mono
    
    float midLevel, sideLevel;
    calculateMidSide(buffer, midLevel, sideLevel);
    
    if (midLevel < 1.0e-6f)
        return 10.0f; // Avoid division by zero, assume very wide
    
    // Ratio of side level to mid level
    return sideLevel / midLevel;
}

void StereoWidthAnalyzer::calculateMidSide(const juce::AudioBuffer<float>& buffer, 
                                         float& midLevel, 
                                         float& sideLevel)
{
    // Need at least stereo for mid/side
    if (buffer.getNumChannels() < 2 || buffer.getNumSamples() == 0)
    {
        midLevel = 1.0f;
        sideLevel = 0.0f;
        return;
    }
    
    const float* leftData = buffer.getReadPointer(0);
    const float* rightData = buffer.getReadPointer(1);
    
    double sumMidSquared = 0.0;
    double sumSideSquared = 0.0;
    
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        float left = leftData[i];
        float right = rightData[i];
        
        float mid = (left + right) * 0.5f;
        float side = (left - right) * 0.5f;
        
        sumMidSquared += mid * mid;
        sumSideSquared += side * side;
    }
    
    // Calculate RMS levels
    midLevel = static_cast<float>(std::sqrt(sumMidSquared / buffer.getNumSamples()));
    sideLevel = static_cast<float>(std::sqrt(sumSideSquared / buffer.getNumSamples()));
}

} // namespace ForensEQ
