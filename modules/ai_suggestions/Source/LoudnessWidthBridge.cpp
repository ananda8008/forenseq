#include "LoudnessWidthBridge.h"

namespace ForensEQ {

LoudnessWidthBridge::LoudnessWidthBridge()
{
}

LoudnessWidthBridge::~LoudnessWidthBridge()
{
}

void LoudnessWidthBridge::processLoudnessData(float userLUFS, float referenceLUFS,
                                            float userRMS, float referenceRMS,
                                            const juce::String& stemName)
{
    // Forward the data to the suggestion analyzer
    if (suggestionAnalyzer != nullptr)
    {
        suggestionAnalyzer->analyzeLoudnessData(userLUFS, referenceLUFS, userRMS, referenceRMS, stemName);
    }
}

void LoudnessWidthBridge::processWidthData(float userWidth, float referenceWidth,
                                         const juce::String& stemName)
{
    // Forward the data to the suggestion analyzer
    if (suggestionAnalyzer != nullptr)
    {
        suggestionAnalyzer->analyzeWidthData(userWidth, referenceWidth, stemName);
    }
}

bool LoudnessWidthBridge::applySuggestion(const MixSuggestion& suggestion)
{
    // Handle loudness and stereo width suggestions
    if (suggestion.getType() != SuggestionType::Loudness && 
        suggestion.getType() != SuggestionType::StereoWidth)
        return false;
    
    // In a real implementation, this would communicate with the Loudness/Width module
    // to apply the suggested change
    
    // For now, just log the action
    juce::Logger::writeToLog("Applied " + 
                           MixSuggestion::suggestionTypeToString(suggestion.getType()) + 
                           " suggestion: " + suggestion.getDescription() + 
                           " to " + suggestion.getStemName());
    
    return true;
}

float LoudnessWidthBridge::parseLoudnessFromDescription(const juce::String& description)
{
    // Extract LUFS value from descriptions like:
    // "Reduce level by 2.5 LUFS (currently 2.5 LUFS too hot)"
    // "Increase level by 1.8 LUFS (currently 1.8 LUFS too quiet)"
    
    float lufsValue = 0.0f;
    
    // Look for "LUFS" in the description
    int lufsPos = description.indexOf("LUFS");
    
    if (lufsPos > 0)
    {
        // Extract the number before "LUFS"
        int startPos = lufsPos - 1;
        while (startPos > 0 && (juce::CharacterFunctions::isDigit(description[startPos]) || 
                               description[startPos] == '.'))
        {
            startPos--;
        }
        
        juce::String lufsStr = description.substring(startPos + 1, lufsPos);
        lufsValue = lufsStr.getFloatValue();
        
        // Adjust sign based on increase or reduce
        if (description.contains("Reduce"))
        {
            lufsValue = -lufsValue;
        }
    }
    
    return lufsValue;
}

float LoudnessWidthBridge::parseWidthFromDescription(const juce::String& description)
{
    // Extract width percentage from descriptions like:
    // "Narrow stereo image by 15% (currently too wide)"
    // "Widen stereo image by 20% (currently too narrow)"
    
    float widthValue = 0.0f;
    
    // Look for "%" in the description
    int percentPos = description.indexOf("%");
    
    if (percentPos > 0)
    {
        // Extract the number before "%"
        int startPos = percentPos - 1;
        while (startPos > 0 && juce::CharacterFunctions::isDigit(description[startPos]))
        {
            startPos--;
        }
        
        juce::String widthStr = description.substring(startPos + 1, percentPos);
        widthValue = widthStr.getFloatValue();
        
        // Adjust sign based on widen or narrow
        if (description.contains("Narrow"))
        {
            widthValue = -widthValue;
        }
    }
    
    return widthValue;
}

} // namespace ForensEQ
