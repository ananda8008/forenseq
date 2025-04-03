#include "EQVisualizerBridge.h"

namespace ForensEQ {

EQVisualizerBridge::EQVisualizerBridge()
{
}

EQVisualizerBridge::~EQVisualizerBridge()
{
}

void EQVisualizerBridge::processEQData(const juce::Array<float>& userEQData, 
                                     const juce::Array<float>& referenceEQData,
                                     const juce::String& stemName)
{
    // Forward the data to the suggestion analyzer
    if (suggestionAnalyzer != nullptr)
    {
        suggestionAnalyzer->analyzeEQData(userEQData, referenceEQData, stemName);
    }
}

bool EQVisualizerBridge::applySuggestion(const MixSuggestion& suggestion)
{
    // Only handle EQ balance suggestions
    if (suggestion.getType() != SuggestionType::EQBalance)
        return false;
    
    // Parse the suggestion description to extract frequency and dB value
    float frequency = parseFrequencyFromDescription(suggestion.getDescription());
    float dbValue = parseDBValueFromDescription(suggestion.getDescription());
    
    if (frequency <= 0.0f || dbValue == 0.0f)
        return false;
    
    // In a real implementation, this would communicate with the EQ visualizer
    // to apply the suggested EQ change
    
    // For now, just log the action
    juce::Logger::writeToLog("Applied EQ suggestion: " + suggestion.getDescription() + 
                           " to " + suggestion.getStemName() + 
                           " (Frequency: " + juce::String(frequency) + 
                           "Hz, Value: " + juce::String(dbValue) + "dB)");
    
    return true;
}

float EQVisualizerBridge::parseFrequencyFromDescription(const juce::String& description)
{
    // Extract frequency from descriptions like:
    // "Boost around 100Hz by 2.5dB"
    // "Cut around 3.5kHz by 1.8dB"
    
    float frequency = 0.0f;
    
    // Look for Hz or kHz in the description
    int hzPos = description.indexOf("Hz");
    int khzPos = description.indexOf("kHz");
    
    if (hzPos > 0)
    {
        // Extract the number before "Hz"
        int startPos = hzPos - 1;
        while (startPos > 0 && (juce::CharacterFunctions::isDigit(description[startPos]) || 
                               description[startPos] == '.'))
        {
            startPos--;
        }
        
        juce::String freqStr = description.substring(startPos + 1, hzPos);
        frequency = freqStr.getFloatValue();
    }
    else if (khzPos > 0)
    {
        // Extract the number before "kHz" and multiply by 1000
        int startPos = khzPos - 1;
        while (startPos > 0 && (juce::CharacterFunctions::isDigit(description[startPos]) || 
                               description[startPos] == '.'))
        {
            startPos--;
        }
        
        juce::String freqStr = description.substring(startPos + 1, khzPos);
        frequency = freqStr.getFloatValue() * 1000.0f;
    }
    
    return frequency;
}

float EQVisualizerBridge::parseDBValueFromDescription(const juce::String& description)
{
    // Extract dB value from descriptions like:
    // "Boost around 100Hz by 2.5dB"
    // "Cut around 3.5kHz by 1.8dB"
    
    float dbValue = 0.0f;
    
    // Look for "dB" in the description
    int dbPos = description.indexOf("dB");
    
    if (dbPos > 0)
    {
        // Extract the number before "dB"
        int startPos = dbPos - 1;
        while (startPos > 0 && (juce::CharacterFunctions::isDigit(description[startPos]) || 
                               description[startPos] == '.'))
        {
            startPos--;
        }
        
        juce::String dbStr = description.substring(startPos + 1, dbPos);
        dbValue = dbStr.getFloatValue();
        
        // Adjust sign based on boost or cut
        if (description.contains("Cut"))
        {
            dbValue = -dbValue;
        }
    }
    
    return dbValue;
}

} // namespace ForensEQ
