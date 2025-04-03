#include "SuggestionAnalyzer.h"

namespace ForensEQ {

SuggestionAnalyzer::SuggestionAnalyzer()
{
}

SuggestionAnalyzer::~SuggestionAnalyzer()
{
}

void SuggestionAnalyzer::analyzeEQData(const juce::Array<float>& userEQData, 
                                     const juce::Array<float>& referenceEQData,
                                     const juce::String& stemName)
{
    // Store data for advanced analysis
    int stemIndex = findStemIndex(stemName);
    
    if (stemIndex >= 0)
    {
        // Update existing stem data
        stemAnalysisData.getReference(stemIndex).userEQData = userEQData;
        stemAnalysisData.getReference(stemIndex).referenceEQData = referenceEQData;
    }
    else
    {
        // Add new stem data
        StemAnalysisData data;
        data.stemName = stemName;
        data.userEQData = userEQData;
        data.referenceEQData = referenceEQData;
        stemAnalysisData.add(data);
    }
    
    // Generate basic EQ suggestions
    if (suggestionEngine != nullptr)
    {
        suggestionEngine->generateEQSuggestions(userEQData, referenceEQData, stemName);
    }
}

void SuggestionAnalyzer::analyzeLoudnessData(float userLUFS, float referenceLUFS,
                                           float userRMS, float referenceRMS,
                                           const juce::String& stemName)
{
    // Store data for advanced analysis
    int stemIndex = findStemIndex(stemName);
    
    if (stemIndex >= 0)
    {
        // Update existing stem data
        stemAnalysisData.getReference(stemIndex).userLUFS = userLUFS;
        stemAnalysisData.getReference(stemIndex).referenceLUFS = referenceLUFS;
        stemAnalysisData.getReference(stemIndex).userRMS = userRMS;
        stemAnalysisData.getReference(stemIndex).referenceRMS = referenceRMS;
    }
    else
    {
        // Add new stem data
        StemAnalysisData data;
        data.stemName = stemName;
        data.userLUFS = userLUFS;
        data.referenceLUFS = referenceLUFS;
        data.userRMS = userRMS;
        data.referenceRMS = referenceRMS;
        stemAnalysisData.add(data);
    }
    
    // Generate basic loudness suggestions
    if (suggestionEngine != nullptr)
    {
        suggestionEngine->generateLoudnessSuggestions(userLUFS, referenceLUFS, userRMS, referenceRMS, stemName);
    }
}

void SuggestionAnalyzer::analyzeWidthData(float userWidth, float referenceWidth,
                                        const juce::String& stemName)
{
    // Store data for advanced analysis
    int stemIndex = findStemIndex(stemName);
    
    if (stemIndex >= 0)
    {
        // Update existing stem data
        stemAnalysisData.getReference(stemIndex).userWidth = userWidth;
        stemAnalysisData.getReference(stemIndex).referenceWidth = referenceWidth;
    }
    else
    {
        // Add new stem data
        StemAnalysisData data;
        data.stemName = stemName;
        data.userWidth = userWidth;
        data.referenceWidth = referenceWidth;
        stemAnalysisData.add(data);
    }
    
    // Generate basic width suggestions
    if (suggestionEngine != nullptr)
    {
        suggestionEngine->generateWidthSuggestions(userWidth, referenceWidth, stemName);
    }
}

void SuggestionAnalyzer::analyzeAllStems(const juce::var& stemAnalysisData)
{
    // Parse JSON data from stem analysis module
    if (stemAnalysisData.isObject())
    {
        // Clear existing suggestions
        if (suggestionEngine != nullptr)
        {
            suggestionEngine->clearSuggestions();
        }
        
        // Process each stem
        if (auto* stems = stemAnalysisData.getProperty("stems", {}).getDynamicObject())
        {
            // Process Full Mix
            if (auto* fullMix = stems->getProperty("FullMix").getDynamicObject())
            {
                analyzeStemFromJSON("Full Mix", fullMix);
            }
            
            // Process Kick
            if (auto* kick = stems->getProperty("Kick").getDynamicObject())
            {
                analyzeStemFromJSON("Kick", kick);
            }
            
            // Process Snare
            if (auto* snare = stems->getProperty("Snare").getDynamicObject())
            {
                analyzeStemFromJSON("Snare", snare);
            }
            
            // Process Bass
            if (auto* bass = stems->getProperty("Bass").getDynamicObject())
            {
                analyzeStemFromJSON("Bass", bass);
            }
            
            // Process Vocals
            if (auto* vocals = stems->getProperty("Vocals").getDynamicObject())
            {
                analyzeStemFromJSON("Vocals", vocals);
            }
            
            // Process Other
            if (auto* other = stems->getProperty("Other").getDynamicObject())
            {
                analyzeStemFromJSON("Other", other);
            }
        }
        
        // Generate advanced suggestions based on all stems
        generateAdvancedSuggestions();
    }
}

void SuggestionAnalyzer::generateAdvancedSuggestions()
{
    // Only proceed if we have data for multiple stems
    if (stemAnalysisData.size() < 2 || suggestionEngine == nullptr)
        return;
    
    // Generate advanced suggestions based on relationships between stems
    generateFrequencyMaskingSuggestions();
    generateDynamicBalanceSuggestions();
    generateStereoImagingSuggestions();
}

void SuggestionAnalyzer::generateFrequencyMaskingSuggestions()
{
    // Check for frequency masking between stems
    // This is a simplified implementation - a real one would use more sophisticated analysis
    
    // Check for bass and kick interaction
    int bassIndex = findStemIndex("Bass");
    int kickIndex = findStemIndex("Kick");
    
    if (bassIndex >= 0 && kickIndex >= 0)
    {
        // Check for potential masking in the low end (around 80-120Hz)
        // In a real implementation, this would analyze the actual frequency content
        
        MixSuggestion suggestion(
            "Bass",
            SuggestionType::EQBalance,
            "Consider cutting Bass around 100Hz to reduce masking with Kick",
            SuggestionPriority::Medium
        );
        
        suggestionEngine->getSuggestionManager().addSuggestion(suggestion);
    }
    
    // Check for vocals and other elements interaction
    int vocalsIndex = findStemIndex("Vocals");
    
    if (vocalsIndex >= 0)
    {
        // Check for potential masking in the mid range (around 1-3kHz)
        // In a real implementation, this would analyze the actual frequency content
        
        MixSuggestion suggestion(
            "Vocals",
            SuggestionType::EQBalance,
            "Try a small boost around 2-3kHz to help vocals cut through the mix",
            SuggestionPriority::Medium
        );
        
        suggestionEngine->getSuggestionManager().addSuggestion(suggestion);
    }
}

void SuggestionAnalyzer::generateDynamicBalanceSuggestions()
{
    // Check for dynamic balance issues between stems
    // This is a simplified implementation - a real one would use more sophisticated analysis
    
    // Check for overall dynamic balance
    int fullMixIndex = findStemIndex("Full Mix");
    
    if (fullMixIndex >= 0)
    {
        // In a real implementation, this would analyze the actual dynamic range
        
        MixSuggestion suggestion(
            "Full Mix",
            SuggestionType::Dynamics,
            "Consider adding light compression to improve overall dynamic balance",
            SuggestionPriority::Low
        );
        
        suggestionEngine->getSuggestionManager().addSuggestion(suggestion);
    }
}

void SuggestionAnalyzer::generateStereoImagingSuggestions()
{
    // Check for stereo imaging issues between stems
    // This is a simplified implementation - a real one would use more sophisticated analysis
    
    // Check for kick and bass stereo width
    int bassIndex = findStemIndex("Bass");
    int kickIndex = findStemIndex("Kick");
    
    if (bassIndex >= 0 && kickIndex >= 0)
    {
        // In a real implementation, this would analyze the actual stereo content
        
        MixSuggestion suggestion(
            "Kick",
            SuggestionType::StereoWidth,
            "Keep kick centered (mono) for maximum impact and phase coherence",
            SuggestionPriority::Medium
        );
        
        suggestionEngine->getSuggestionManager().addSuggestion(suggestion);
    }
    
    // Check for overall stereo imaging
    int fullMixIndex = findStemIndex("Full Mix");
    
    if (fullMixIndex >= 0)
    {
        // In a real implementation, this would analyze the actual stereo content
        
        MixSuggestion suggestion(
            "Full Mix",
            SuggestionType::StereoWidth,
            "Consider widening high frequencies (>5kHz) for a more spacious mix",
            SuggestionPriority::Low
        );
        
        suggestionEngine->getSuggestionManager().addSuggestion(suggestion);
    }
}

int SuggestionAnalyzer::findStemIndex(const juce::String& stemName)
{
    for (int i = 0; i < stemAnalysisData.size(); ++i)
    {
        if (stemAnalysisData[i].stemName == stemName)
        {
            return i;
        }
    }
    
    return -1;
}

void SuggestionAnalyzer::analyzeStemFromJSON(const juce::String& stemName, const juce::DynamicObject* stemData)
{
    if (stemData == nullptr || suggestionEngine == nullptr)
        return;
    
    // Extract loudness data
    float userLUFS = stemData->getProperty("userIntegratedLUFS");
    float referenceLUFS = stemData->getProperty("referenceIntegratedLUFS");
    float userRMS = -20.0f; // Default value, would be extracted from actual data
    float referenceRMS = -20.0f; // Default value, would be extracted from actual data
    
    // Extract width data
    float userWidth = stemData->getProperty("userWidthPercentage");
    float referenceWidth = stemData->getProperty("referenceWidthPercentage");
    
    // Generate suggestions
    suggestionEngine->generateLoudnessSuggestions(userLUFS, referenceLUFS, userRMS, referenceRMS, stemName);
    suggestionEngine->generateWidthSuggestions(userWidth, referenceWidth, stemName);
    
    // EQ data would require more complex extraction and processing
    // This is a simplified implementation
}

} // namespace ForensEQ
