#include "SuggestionEngine.h"

namespace ForensEQ {

SuggestionEngine::SuggestionEngine()
{
}

SuggestionEngine::~SuggestionEngine()
{
}

void SuggestionEngine::generateEQSuggestions(const juce::Array<float>& userEQData, 
                                           const juce::Array<float>& referenceEQData,
                                           const juce::String& stemName)
{
    // Ensure the arrays have the same size
    if (userEQData.size() != referenceEQData.size() || userEQData.isEmpty())
        return;
    
    // Analyze EQ differences at key frequency bands
    for (int i = 0; i < juce::jmin(userEQData.size(), frequencyBands.size()); ++i)
    {
        float difference = userEQData[i] - referenceEQData[i];
        
        // If the difference exceeds the threshold, generate a suggestion
        if (std::abs(difference) > eqThreshold)
        {
            // Create a suggestion
            MixSuggestion suggestion(
                stemName,
                SuggestionType::EQBalance,
                generateEQDescription(frequencyBands[i], difference),
                getPriorityFromDifference(std::abs(difference), eqThreshold)
            );
            
            // Add to suggestion manager
            suggestionManager.addSuggestion(suggestion);
        }
    }
}

void SuggestionEngine::generateLoudnessSuggestions(float userLUFS, float referenceLUFS,
                                                 float userRMS, float referenceRMS,
                                                 const juce::String& stemName)
{
    // Calculate differences
    float lufsDifference = userLUFS - referenceLUFS;
    float rmsDifference = userRMS - referenceRMS;
    
    // If the LUFS difference exceeds the threshold, generate a suggestion
    if (std::abs(lufsDifference) > loudnessThreshold)
    {
        // Create a suggestion
        MixSuggestion suggestion(
            stemName,
            SuggestionType::Loudness,
            generateLoudnessDescription(lufsDifference, rmsDifference),
            getPriorityFromDifference(std::abs(lufsDifference), loudnessThreshold)
        );
        
        // Add to suggestion manager
        suggestionManager.addSuggestion(suggestion);
    }
}

void SuggestionEngine::generateWidthSuggestions(float userWidth, float referenceWidth,
                                              const juce::String& stemName)
{
    // Calculate difference
    float widthDifference = userWidth - referenceWidth;
    
    // If the width difference exceeds the threshold, generate a suggestion
    if (std::abs(widthDifference) > widthThreshold)
    {
        // Create a suggestion
        MixSuggestion suggestion(
            stemName,
            SuggestionType::StereoWidth,
            generateWidthDescription(widthDifference),
            getPriorityFromDifference(std::abs(widthDifference), widthThreshold)
        );
        
        // Add to suggestion manager
        suggestionManager.addSuggestion(suggestion);
    }
}

juce::String SuggestionEngine::generateEQDescription(float frequency, float difference)
{
    // Format the frequency in a human-readable way
    juce::String freqStr;
    
    if (frequency < 1000)
    {
        freqStr = juce::String(static_cast<int>(frequency)) + "Hz";
    }
    else
    {
        freqStr = juce::String(frequency / 1000.0f, 1) + "kHz";
    }
    
    // Generate description based on the difference
    if (difference > 0)
    {
        return "Cut around " + freqStr + " by " + juce::String(std::abs(difference), 1) + "dB";
    }
    else
    {
        return "Boost around " + freqStr + " by " + juce::String(std::abs(difference), 1) + "dB";
    }
}

juce::String SuggestionEngine::generateLoudnessDescription(float lufsDifference, float rmsDifference)
{
    // Generate description based on the LUFS difference
    if (lufsDifference > 0)
    {
        return "Reduce level by " + juce::String(std::abs(lufsDifference), 1) + " LUFS (currently " + 
               juce::String(std::abs(lufsDifference), 1) + " LUFS too hot)";
    }
    else
    {
        return "Increase level by " + juce::String(std::abs(lufsDifference), 1) + " LUFS (currently " + 
               juce::String(std::abs(lufsDifference), 1) + " LUFS too quiet)";
    }
}

juce::String SuggestionEngine::generateWidthDescription(float widthDifference)
{
    // Generate description based on the width difference
    if (widthDifference > 0)
    {
        return "Narrow stereo image by " + juce::String(static_cast<int>(std::abs(widthDifference))) + 
               "% (currently too wide)";
    }
    else
    {
        return "Widen stereo image by " + juce::String(static_cast<int>(std::abs(widthDifference))) + 
               "% (currently too narrow)";
    }
}

SuggestionPriority SuggestionEngine::getPriorityFromDifference(float difference, float threshold)
{
    // Determine priority based on how much the difference exceeds the threshold
    float ratio = difference / threshold;
    
    if (ratio > 2.5f)
    {
        return SuggestionPriority::High;
    }
    else if (ratio > 1.5f)
    {
        return SuggestionPriority::Medium;
    }
    else
    {
        return SuggestionPriority::Low;
    }
}

} // namespace ForensEQ
