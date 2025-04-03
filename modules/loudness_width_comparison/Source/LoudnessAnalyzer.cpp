#include "LoudnessAnalyzer.h"

namespace ForensEQ {

LoudnessAnalyzer::LoudnessAnalyzer()
{
}

LoudnessAnalyzer::~LoudnessAnalyzer()
{
}

float LoudnessAnalyzer::calculateLoudness(const juce::AudioBuffer<float>& buffer, 
                                         LoudnessType type, 
                                         double sampleRate)
{
    switch (type)
    {
        case LoudnessType::Integrated:
            return calculateIntegratedLUFS(buffer, sampleRate);
        case LoudnessType::ShortTerm:
            return calculateShortTermLUFS(buffer, sampleRate);
        case LoudnessType::Momentary:
            return calculateMomentaryLUFS(buffer, sampleRate);
        case LoudnessType::RMS:
            return calculateRMS(buffer);
        default:
            return 0.0f;
    }
}

float LoudnessAnalyzer::calculateLoudnessDifference(const juce::AudioBuffer<float>& userBuffer,
                                                  const juce::AudioBuffer<float>& referenceBuffer,
                                                  LoudnessType type,
                                                  double sampleRate)
{
    float userLoudness = calculateLoudness(userBuffer, type, sampleRate);
    float referenceLoudness = calculateLoudness(referenceBuffer, type, sampleRate);
    
    // Return the difference (positive means user is louder than reference)
    return userLoudness - referenceLoudness;
}

juce::String LoudnessAnalyzer::getLoudnessDifferenceDescription(float difference, LoudnessType type)
{
    juce::String unit;
    
    switch (type)
    {
        case LoudnessType::Integrated:
        case LoudnessType::ShortTerm:
        case LoudnessType::Momentary:
            unit = "LUFS";
            break;
        case LoudnessType::RMS:
            unit = "dB";
            break;
    }
    
    if (std::abs(difference) < 0.5f)
    {
        return "Matched";
    }
    else if (difference > 0)
    {
        return "+" + juce::String(difference, 1) + " " + unit + " too hot";
    }
    else
    {
        return juce::String(difference, 1) + " " + unit + " too quiet";
    }
}

juce::Colour LoudnessAnalyzer::getLoudnessDifferenceColor(float difference, LoudnessType type)
{
    // Convert difference to absolute value for color calculation
    float absDifference = std::abs(difference);
    
    // Perfect match (green)
    if (absDifference < perfectMatchThreshold)
    {
        return juce::Colour(0, 255, 0);
    }
    // Good match (yellow-green)
    else if (absDifference < goodMatchThreshold)
    {
        float t = (absDifference - perfectMatchThreshold) / (goodMatchThreshold - perfectMatchThreshold);
        return juce::Colour(
            static_cast<uint8_t>(t * 255),  // R: 0 -> 255
            static_cast<uint8_t>(255),      // G: 255 -> 255
            static_cast<uint8_t>(0)         // B: 0 -> 0
        );
    }
    // Fair match (yellow to orange)
    else if (absDifference < fairMatchThreshold)
    {
        float t = (absDifference - goodMatchThreshold) / (fairMatchThreshold - goodMatchThreshold);
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

float LoudnessAnalyzer::calculateLoudnessMatchScore(const juce::AudioBuffer<float>& userBuffer,
                                                  const juce::AudioBuffer<float>& referenceBuffer,
                                                  LoudnessType type,
                                                  double sampleRate)
{
    float difference = std::abs(calculateLoudnessDifference(userBuffer, referenceBuffer, type, sampleRate));
    
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
        return 0.5f * std::exp(-(difference - fairMatchThreshold) / 5.0f);
    }
}

float LoudnessAnalyzer::calculateIntegratedLUFS(const juce::AudioBuffer<float>& buffer, double sampleRate)
{
    // This is a simplified implementation of the ITU-R BS.1770-4 algorithm
    // A full implementation would require multiple stages of filtering and gating
    
    if (buffer.getNumSamples() == 0)
        return -70.0f; // Silent
    
    // Apply K-weighting filter (simplified)
    juce::AudioBuffer<float> filteredBuffer(buffer.getNumChannels(), buffer.getNumSamples());
    filteredBuffer.copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
    
    // Calculate mean square for each channel
    double sum = 0.0;
    int numChannels = buffer.getNumChannels();
    
    for (int channel = 0; channel < numChannels; ++channel)
    {
        const float* channelData = buffer.getReadPointer(channel);
        
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float sample = channelData[i];
            sum += sample * sample;
        }
    }
    
    // Calculate mean square
    double meanSquare = sum / (buffer.getNumSamples() * numChannels);
    
    // Convert to LUFS (simplified)
    return 10.0f * std::log10(meanSquare) - 0.691f;
}

float LoudnessAnalyzer::calculateShortTermLUFS(const juce::AudioBuffer<float>& buffer, double sampleRate)
{
    // Short-term LUFS uses a 3-second window
    // This is a simplified implementation
    
    return calculateIntegratedLUFS(buffer, sampleRate) + 0.5f; // Slight adjustment for demonstration
}

float LoudnessAnalyzer::calculateMomentaryLUFS(const juce::AudioBuffer<float>& buffer, double sampleRate)
{
    // Momentary LUFS uses a 400ms window
    // This is a simplified implementation
    
    return calculateIntegratedLUFS(buffer, sampleRate) + 1.0f; // Slight adjustment for demonstration
}

float LoudnessAnalyzer::calculateRMS(const juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumSamples() == 0)
        return -70.0f; // Silent
    
    double sum = 0.0;
    int numChannels = buffer.getNumChannels();
    
    for (int channel = 0; channel < numChannels; ++channel)
    {
        const float* channelData = buffer.getReadPointer(channel);
        
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float sample = channelData[i];
            sum += sample * sample;
        }
    }
    
    // Calculate RMS
    double rms = std::sqrt(sum / (buffer.getNumSamples() * numChannels));
    
    // Convert to dB
    return 20.0f * std::log10(rms + 1.0e-6f);
}

} // namespace ForensEQ
