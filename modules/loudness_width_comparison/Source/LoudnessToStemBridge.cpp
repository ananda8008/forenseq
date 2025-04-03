#include "LoudnessToStemBridge.h"

namespace ForensEQ {

LoudnessToStemBridge::LoudnessToStemBridge()
{
}

LoudnessToStemBridge::~LoudnessToStemBridge()
{
}

void LoudnessToStemBridge::setComparisonResult(const ComparisonResult& result)
{
    currentResult = result;
}

juce::Array<LoudnessToStemBridge::StemLoudnessData> LoudnessToStemBridge::getAllStemLoudnessData() const
{
    juce::Array<StemLoudnessData> result;
    
    // Collect loudness data for all stem types
    for (int i = 0; i < 6; ++i)
    {
        ComparisonResult::StemType stemType = static_cast<ComparisonResult::StemType>(i);
        
        StemLoudnessData data;
        data.stemType = stemType;
        data.userIntegratedLUFS = currentResult.getUserLoudness(stemType, LoudnessAnalyzer::LoudnessType::Integrated);
        data.referenceIntegratedLUFS = currentResult.getReferenceLoudness(stemType, LoudnessAnalyzer::LoudnessType::Integrated);
        data.loudnessDifference = currentResult.getLoudnessDifference(stemType, LoudnessAnalyzer::LoudnessType::Integrated);
        data.matchScore = currentResult.getLoudnessMatchScore(stemType, LoudnessAnalyzer::LoudnessType::Integrated);
        
        result.add(data);
    }
    
    return result;
}

juce::Array<LoudnessToStemBridge::StemWidthData> LoudnessToStemBridge::getAllStemWidthData() const
{
    juce::Array<StemWidthData> result;
    
    // Collect width data for all stem types
    for (int i = 0; i < 6; ++i)
    {
        ComparisonResult::StemType stemType = static_cast<ComparisonResult::StemType>(i);
        
        StemWidthData data;
        data.stemType = stemType;
        data.userWidthPercentage = currentResult.getUserWidth(stemType, StereoWidthAnalyzer::WidthType::Percentage);
        data.referenceWidthPercentage = currentResult.getReferenceWidth(stemType, StereoWidthAnalyzer::WidthType::Percentage);
        data.widthDifference = currentResult.getWidthDifference(stemType, StereoWidthAnalyzer::WidthType::Percentage);
        data.matchScore = currentResult.getWidthMatchScore(stemType, StereoWidthAnalyzer::WidthType::Percentage);
        
        result.add(data);
    }
    
    return result;
}

juce::Array<LoudnessToStemBridge::StemMatchData> LoudnessToStemBridge::getAllStemMatchData() const
{
    juce::Array<StemMatchData> result;
    
    // Collect match data for all stem types
    for (int i = 0; i < 6; ++i)
    {
        ComparisonResult::StemType stemType = static_cast<ComparisonResult::StemType>(i);
        
        StemMatchData data;
        data.stemType = stemType;
        data.combinedMatchScore = currentResult.getCombinedMatchScore(stemType);
        data.matchDescription = getMatchDescription(data.combinedMatchScore);
        
        result.add(data);
    }
    
    return result;
}

juce::String LoudnessToStemBridge::getAISuggestionData() const
{
    // Create a JSON object with all the data needed for AI suggestions
    juce::DynamicObject::Ptr rootObject = new juce::DynamicObject();
    
    // Add stem data
    juce::DynamicObject::Ptr stemsObject = new juce::DynamicObject();
    
    for (int i = 0; i < 6; ++i)
    {
        ComparisonResult::StemType stemType = static_cast<ComparisonResult::StemType>(i);
        juce::String stemName = getStemTypeName(stemType);
        
        juce::DynamicObject::Ptr stemObject = new juce::DynamicObject();
        
        // Add loudness data
        stemObject->setProperty("userIntegratedLUFS", 
                               currentResult.getUserLoudness(stemType, LoudnessAnalyzer::LoudnessType::Integrated));
        stemObject->setProperty("referenceIntegratedLUFS", 
                               currentResult.getReferenceLoudness(stemType, LoudnessAnalyzer::LoudnessType::Integrated));
        stemObject->setProperty("loudnessDifference", 
                               currentResult.getLoudnessDifference(stemType, LoudnessAnalyzer::LoudnessType::Integrated));
        stemObject->setProperty("loudnessMatchScore", 
                               currentResult.getLoudnessMatchScore(stemType, LoudnessAnalyzer::LoudnessType::Integrated));
        
        // Add width data
        stemObject->setProperty("userWidthPercentage", 
                               currentResult.getUserWidth(stemType, StereoWidthAnalyzer::WidthType::Percentage));
        stemObject->setProperty("referenceWidthPercentage", 
                               currentResult.getReferenceWidth(stemType, StereoWidthAnalyzer::WidthType::Percentage));
        stemObject->setProperty("widthDifference", 
                               currentResult.getWidthDifference(stemType, StereoWidthAnalyzer::WidthType::Percentage));
        stemObject->setProperty("widthMatchScore", 
                               currentResult.getWidthMatchScore(stemType, StereoWidthAnalyzer::WidthType::Percentage));
        
        // Add combined match score
        stemObject->setProperty("combinedMatchScore", currentResult.getCombinedMatchScore(stemType));
        
        // Add to stems object
        stemsObject->setProperty(stemName, juce::var(stemObject.get()));
    }
    
    rootObject->setProperty("stems", juce::var(stemsObject.get()));
    
    // Add overall analysis
    juce::DynamicObject::Ptr analysisObject = new juce::DynamicObject();
    
    // Calculate overall match score (average of all stem match scores)
    float overallMatchScore = 0.0f;
    for (int i = 0; i < 6; ++i)
    {
        ComparisonResult::StemType stemType = static_cast<ComparisonResult::StemType>(i);
        overallMatchScore += currentResult.getCombinedMatchScore(stemType);
    }
    overallMatchScore /= 6.0f;
    
    analysisObject->setProperty("overallMatchScore", overallMatchScore);
    analysisObject->setProperty("overallMatchDescription", getMatchDescription(overallMatchScore));
    
    // Find the stem with the lowest match score
    float lowestMatchScore = 1.0f;
    ComparisonResult::StemType lowestMatchStem = ComparisonResult::StemType::FullMix;
    
    for (int i = 0; i < 6; ++i)
    {
        ComparisonResult::StemType stemType = static_cast<ComparisonResult::StemType>(i);
        float matchScore = currentResult.getCombinedMatchScore(stemType);
        
        if (matchScore < lowestMatchScore)
        {
            lowestMatchScore = matchScore;
            lowestMatchStem = stemType;
        }
    }
    
    analysisObject->setProperty("lowestMatchStem", getStemTypeName(lowestMatchStem));
    analysisObject->setProperty("lowestMatchScore", lowestMatchScore);
    
    // Add analysis to root object
    rootObject->setProperty("analysis", juce::var(analysisObject.get()));
    
    // Add metadata
    juce::DynamicObject::Ptr metadataObject = new juce::DynamicObject();
    metadataObject->setProperty("version", "1.0");
    metadataObject->setProperty("timestamp", juce::Time::getCurrentTime().toISO8601(true));
    
    rootObject->setProperty("metadata", juce::var(metadataObject.get()));
    
    return juce::JSON::toString(juce::var(rootObject.get()));
}

juce::String LoudnessToStemBridge::getStemTypeName(ComparisonResult::StemType type) const
{
    switch (type)
    {
        case ComparisonResult::StemType::FullMix:
            return "FullMix";
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

juce::String LoudnessToStemBridge::getMatchDescription(float score) const
{
    if (score >= 0.95f)
        return "Perfect Match";
    else if (score >= 0.85f)
        return "Excellent Match";
    else if (score >= 0.75f)
        return "Very Good Match";
    else if (score >= 0.65f)
        return "Good Match";
    else if (score >= 0.55f)
        return "Fair Match";
    else if (score >= 0.45f)
        return "Moderate Differences";
    else if (score >= 0.35f)
        return "Significant Differences";
    else if (score >= 0.25f)
        return "Major Differences";
    else
        return "Extreme Differences";
}

} // namespace ForensEQ
