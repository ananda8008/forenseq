#include "StemAnalysisBridge.h"

namespace ForensEQ {

StemAnalysisBridge::StemAnalysisBridge()
{
}

StemAnalysisBridge::~StemAnalysisBridge()
{
}

void StemAnalysisBridge::processStemData(const juce::var& stemData)
{
    // Forward the data to the suggestion analyzer
    if (suggestionAnalyzer != nullptr)
    {
        suggestionAnalyzer->analyzeAllStems(stemData);
    }
}

bool StemAnalysisBridge::applySuggestion(const MixSuggestion& suggestion)
{
    // Handle suggestions that would affect stem isolation or processing
    // This is a simplified implementation - a real one would communicate with the Stem Analysis module
    
    // For now, just log the action
    juce::Logger::writeToLog("Applied Stem suggestion: " + suggestion.getDescription() + 
                           " to " + suggestion.getStemName());
    
    // Set the active stem to the one mentioned in the suggestion
    activeStem = suggestion.getStemName();
    
    return true;
}

juce::var StemAnalysisBridge::extractStemData(const juce::var& allStemData, const juce::String& stemName)
{
    // Extract data for a specific stem from the complete stem data
    if (allStemData.isObject())
    {
        if (auto* stems = allStemData.getProperty("stems", {}).getDynamicObject())
        {
            // Convert stem name to the format used in the JSON
            juce::String jsonStemName = stemName;
            jsonStemName = jsonStemName.replace(" ", ""); // Remove spaces
            
            return stems->getProperty(jsonStemName);
        }
    }
    
    return {};
}

} // namespace ForensEQ
