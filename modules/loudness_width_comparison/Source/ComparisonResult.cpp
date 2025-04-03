#include "ComparisonResult.h"

namespace ForensEQ {

ComparisonResult::ComparisonResult()
{
    // Initialize with default values for all stem types
    for (int i = 0; i < 6; ++i)
    {
        StemType stemType = static_cast<StemType>(i);
        loudnessValues[stemType] = LoudnessValues();
        widthValues[stemType] = WidthValues();
    }
}

ComparisonResult::~ComparisonResult()
{
}

void ComparisonResult::setLoudnessValues(StemType stemType, 
                                       float userIntegratedLUFS, 
                                       float referenceIntegratedLUFS,
                                       float userShortTermLUFS,
                                       float referenceShortTermLUFS,
                                       float userMomentaryLUFS,
                                       float referenceMomentaryLUFS,
                                       float userRMS,
                                       float referenceRMS)
{
    LoudnessValues values;
    values.userIntegratedLUFS = userIntegratedLUFS;
    values.referenceIntegratedLUFS = referenceIntegratedLUFS;
    values.userShortTermLUFS = userShortTermLUFS;
    values.referenceShortTermLUFS = referenceShortTermLUFS;
    values.userMomentaryLUFS = userMomentaryLUFS;
    values.referenceMomentaryLUFS = referenceMomentaryLUFS;
    values.userRMS = userRMS;
    values.referenceRMS = referenceRMS;
    
    loudnessValues[stemType] = values;
}

void ComparisonResult::setWidthValues(StemType stemType,
                                    float userCorrelation,
                                    float referenceCorrelation,
                                    float userMidSideRatio,
                                    float referenceMidSideRatio)
{
    WidthValues values;
    values.userCorrelation = userCorrelation;
    values.referenceCorrelation = referenceCorrelation;
    values.userMidSideRatio = userMidSideRatio;
    values.referenceMidSideRatio = referenceMidSideRatio;
    
    widthValues[stemType] = values;
}

float ComparisonResult::getLoudnessDifference(StemType stemType, LoudnessAnalyzer::LoudnessType loudnessType) const
{
    auto it = loudnessValues.find(stemType);
    if (it == loudnessValues.end())
        return 0.0f;
    
    const LoudnessValues& values = it->second;
    
    switch (loudnessType)
    {
        case LoudnessAnalyzer::LoudnessType::Integrated:
            return values.userIntegratedLUFS - values.referenceIntegratedLUFS;
        case LoudnessAnalyzer::LoudnessType::ShortTerm:
            return values.userShortTermLUFS - values.referenceShortTermLUFS;
        case LoudnessAnalyzer::LoudnessType::Momentary:
            return values.userMomentaryLUFS - values.referenceMomentaryLUFS;
        case LoudnessAnalyzer::LoudnessType::RMS:
            return values.userRMS - values.referenceRMS;
        default:
            return 0.0f;
    }
}

float ComparisonResult::getWidthDifference(StemType stemType, StereoWidthAnalyzer::WidthType widthType) const
{
    auto it = widthValues.find(stemType);
    if (it == widthValues.end())
        return 0.0f;
    
    const WidthValues& values = it->second;
    
    switch (widthType)
    {
        case StereoWidthAnalyzer::WidthType::Correlation:
            return values.userCorrelation - values.referenceCorrelation;
        case StereoWidthAnalyzer::WidthType::MidSideRatio:
            return values.userMidSideRatio - values.referenceMidSideRatio;
        case StereoWidthAnalyzer::WidthType::Percentage:
        {
            // Convert correlation to percentage
            StereoWidthAnalyzer analyzer;
            float userPercentage = analyzer.correlationToPercentage(values.userCorrelation);
            float referencePercentage = analyzer.correlationToPercentage(values.referenceCorrelation);
            return userPercentage - referencePercentage;
        }
        default:
            return 0.0f;
    }
}

float ComparisonResult::getLoudnessMatchScore(StemType stemType, LoudnessAnalyzer::LoudnessType loudnessType) const
{
    float difference = std::abs(getLoudnessDifference(stemType, loudnessType));
    return calculateLoudnessMatchScore(difference, loudnessType);
}

float ComparisonResult::getWidthMatchScore(StemType stemType, StereoWidthAnalyzer::WidthType widthType) const
{
    float difference;
    
    switch (widthType)
    {
        case StereoWidthAnalyzer::WidthType::Correlation:
            difference = std::abs(getWidthDifference(stemType, widthType));
            break;
        case StereoWidthAnalyzer::WidthType::MidSideRatio:
            difference = std::abs(getWidthDifference(stemType, widthType)) / 0.5f;
            break;
        case StereoWidthAnalyzer::WidthType::Percentage:
            difference = std::abs(getWidthDifference(stemType, widthType)) / 100.0f;
            break;
        default:
            difference = 1.0f; // Maximum difference
    }
    
    return calculateWidthMatchScore(difference, widthType);
}

float ComparisonResult::getCombinedMatchScore(StemType stemType) const
{
    // Calculate average of loudness and width match scores
    float loudnessScore = getLoudnessMatchScore(stemType, LoudnessAnalyzer::LoudnessType::Integrated);
    float widthScore = getWidthMatchScore(stemType, StereoWidthAnalyzer::WidthType::Percentage);
    
    return (loudnessScore + widthScore) * 0.5f;
}

float ComparisonResult::getUserLoudness(StemType stemType, LoudnessAnalyzer::LoudnessType loudnessType) const
{
    auto it = loudnessValues.find(stemType);
    if (it == loudnessValues.end())
        return -70.0f;
    
    const LoudnessValues& values = it->second;
    
    switch (loudnessType)
    {
        case LoudnessAnalyzer::LoudnessType::Integrated:
            return values.userIntegratedLUFS;
        case LoudnessAnalyzer::LoudnessType::ShortTerm:
            return values.userShortTermLUFS;
        case LoudnessAnalyzer::LoudnessType::Momentary:
            return values.userMomentaryLUFS;
        case LoudnessAnalyzer::LoudnessType::RMS:
            return values.userRMS;
        default:
            return -70.0f;
    }
}

float ComparisonResult::getReferenceLoudness(StemType stemType, LoudnessAnalyzer::LoudnessType loudnessType) const
{
    auto it = loudnessValues.find(stemType);
    if (it == loudnessValues.end())
        return -70.0f;
    
    const LoudnessValues& values = it->second;
    
    switch (loudnessType)
    {
        case LoudnessAnalyzer::LoudnessType::Integrated:
            return values.referenceIntegratedLUFS;
        case LoudnessAnalyzer::LoudnessType::ShortTerm:
            return values.referenceShortTermLUFS;
        case LoudnessAnalyzer::LoudnessType::Momentary:
            return values.referenceMomentaryLUFS;
        case LoudnessAnalyzer::LoudnessType::RMS:
            return values.referenceRMS;
        default:
            return -70.0f;
    }
}

float ComparisonResult::getUserWidth(StemType stemType, StereoWidthAnalyzer::WidthType widthType) const
{
    auto it = widthValues.find(stemType);
    if (it == widthValues.end())
        return (widthType == StereoWidthAnalyzer::WidthType::Correlation) ? 1.0f : 0.0f;
    
    const WidthValues& values = it->second;
    
    switch (widthType)
    {
        case StereoWidthAnalyzer::WidthType::Correlation:
            return values.userCorrelation;
        case StereoWidthAnalyzer::WidthType::MidSideRatio:
            return values.userMidSideRatio;
        case StereoWidthAnalyzer::WidthType::Percentage:
        {
            // Convert correlation to percentage
            StereoWidthAnalyzer analyzer;
            return analyzer.correlationToPercentage(values.userCorrelation);
        }
        default:
            return 0.0f;
    }
}

float ComparisonResult::getReferenceWidth(StemType stemType, StereoWidthAnalyzer::WidthType widthType) const
{
    auto it = widthValues.find(stemType);
    if (it == widthValues.end())
        return (widthType == StereoWidthAnalyzer::WidthType::Correlation) ? 1.0f : 0.0f;
    
    const WidthValues& values = it->second;
    
    switch (widthType)
    {
        case StereoWidthAnalyzer::WidthType::Correlation:
            return values.referenceCorrelation;
        case StereoWidthAnalyzer::WidthType::MidSideRatio:
            return values.referenceMidSideRatio;
        case StereoWidthAnalyzer::WidthType::Percentage:
        {
            // Convert correlation to percentage
            StereoWidthAnalyzer analyzer;
            return analyzer.correlationToPercentage(values.referenceCorrelation);
        }
        default:
            return 0.0f;
    }
}

juce::String ComparisonResult::toJSON() const
{
    juce::DynamicObject::Ptr rootObject = new juce::DynamicObject();
    
    // Create loudness object
    juce::DynamicObject::Ptr loudnessObject = new juce::DynamicObject();
    
    for (const auto& pair : loudnessValues)
    {
        juce::String stemName;
        switch (pair.first)
        {
            case StemType::FullMix: stemName = "FullMix"; break;
            case StemType::Kick: stemName = "Kick"; break;
            case StemType::Snare: stemName = "Snare"; break;
            case StemType::Bass: stemName = "Bass"; break;
            case StemType::Vocals: stemName = "Vocals"; break;
            case StemType::Other: stemName = "Other"; break;
            default: stemName = "Unknown"; break;
        }
        
        juce::DynamicObject::Ptr stemObject = new juce::DynamicObject();
        
        // User values
        juce::DynamicObject::Ptr userObject = new juce::DynamicObject();
        userObject->setProperty("IntegratedLUFS", pair.second.userIntegratedLUFS);
        userObject->setProperty("ShortTermLUFS", pair.second.userShortTermLUFS);
        userObject->setProperty("MomentaryLUFS", pair.second.userMomentaryLUFS);
        userObject->setProperty("RMS", pair.second.userRMS);
        
        // Reference values
        juce::DynamicObject::Ptr referenceObject = new juce::DynamicObject();
        referenceObject->setProperty("IntegratedLUFS", pair.second.referenceIntegratedLUFS);
        referenceObject->setProperty("ShortTermLUFS", pair.second.referenceShortTermLUFS);
        referenceObject->setProperty("MomentaryLUFS", pair.second.referenceMomentaryLUFS);
        referenceObject->setProperty("RMS", pair.second.referenceRMS);
        
        stemObject->setProperty("User", juce::var(userObject.get()));
        stemObject->setProperty("Reference", juce::var(referenceObject.get()));
        
        loudnessObject->setProperty(stemName, juce::var(stemObject.get()));
    }
    
    // Create width object
    juce::DynamicObject::Ptr widthObject = new juce::DynamicObject();
    
    for (const auto& pair : widthValues)
    {
        juce::String stemName;
        switch (pair.first)
        {
            case StemType::FullMix: stemName = "FullMix"; break;
            case StemType::Kick: stemName = "Kick"; break;
            case StemType::Snare: stemName = "Snare"; break;
            case StemType::Bass: stemName = "Bass"; break;
            case StemType::Vocals: stemName = "Vocals"; break;
            case StemType::Other: stemName = "Other"; break;
            default: stemName = "Unknown"; break;
        }
        
        juce::DynamicObject::Ptr stemObject = new juce::DynamicObject();
        
        // User values
        juce::DynamicObject::Ptr userObject = new juce::DynamicObject();
        userObject->setProperty("Correlation", pair.second.userCorrelation);
        userObject->setProperty("MidSideRatio", pair.second.userMidSideRatio);
        
        // Reference values
        juce::DynamicObject::Ptr referenceObject = new juce::DynamicObject();
        referenceObject->setProperty("Correlation", pair.second.referenceCorrelation);
        referenceObject->setProperty("MidSideRatio", pair.second.referenceMidSideRatio);
        
        stemObject->setProperty("User", juce::var(userObject.get()));
        stemObject->setProperty("Reference", juce::var(referenceObject.get()));
        
        widthObject->setProperty(stemName, juce::var(stemObject.get()));
    }
    
    // Add to root object
    rootObject->setProperty("Loudness", juce::var(loudnessObject.get()));
    rootObject->setProperty("Width", juce::var(widthObject.get()));
    
    // Create match scores object
    juce::DynamicObject::Ptr matchScoresObject = new juce::DynamicObject();
    
    for (int i = 0; i < 6; ++i)
    {
        StemType stemType = static_cast<StemType>(i);
        
        juce::String stemName;
        switch (stemType)
        {
            case StemType::FullMix: stemName = "FullMix"; break;
            case StemType::Kick: stemName = "Kick"; break;
            case StemType::Snare: stemName = "Snare"; break;
            case StemType::Bass: stemName = "Bass"; break;
            case StemType::Vocals: stemName = "Vocals"; break;
            case StemType::Other: stemName = "Other"; break;
            default: stemName = "Unknown"; break;
        }
        
        juce::DynamicObject::Ptr stemObject = new juce::DynamicObject();
        
        stemObject->setProperty("LoudnessMatch", getLoudnessMatchScore(stemType, LoudnessAnalyzer::LoudnessType::Integrated));
        stemObject->setProperty("WidthMatch", getWidthMatchScore(stemType, StereoWidthAnalyzer::WidthType::Percentage));
        stemObject->setProperty("CombinedMatch", getCombinedMatchScore(stemType));
        
        matchScoresObject->setProperty(stemName, juce::var(stemObject.get()));
    }
    
    rootObject->setProperty("MatchScores", juce::var(matchScoresObject.get()));
    
    return juce::JSON::toString(juce::var(rootObject.get()));
}

bool ComparisonResult::fromJSON(const juce::String& jsonString)
{
    juce::var parsedJson = juce::JSON::parse(jsonString);
    
    if (!parsedJson.isObject())
        return false;
    
    // Clear existing data
    loudnessValues.clear();
    widthValues.clear();
    
    // Initialize with default values
    for (int i = 0; i < 6; ++i)
    {
        StemType stemType = static_cast<StemType>(i);
        loudnessValues[stemType] = LoudnessValues();
        widthValues[stemType] = WidthValues();
    }
    
    // Parse loudness data
    if (parsedJson.hasProperty("Loudness") && parsedJson["Loudness"].isObject())
    {
        juce::var loudnessVar = parsedJson["Loudness"];
        
        for (int i = 0; i < 6; ++i)
        {
            StemType stemType = static_cast<StemType>(i);
            juce::String stemName;
            
            switch (stemType)
            {
                case StemType::FullMix: stemName = "FullMix"; break;
                case StemType::Kick: stemName = "Kick"; break;
                case StemType::Snare: stemName = "Snare"; break;
                case StemType::Bass: stemName = "Bass"; break;
                case StemType::Vocals: stemName = "Vocals"; break;
                case StemType::Other: stemName = "Other"; break;
                default: stemName = "Unknown"; break;
            }
            
            if (loudnessVar.hasProperty(stemName) && loudnessVar[stemName].isObject())
            {
                juce::var stemVar = loudnessVar[stemName];
                
                if (stemVar.hasProperty("User") && stemVar["User"].isObject() &&
                    stemVar.hasProperty("Reference") && stemVar["Reference"].isObject())
                {
                    juce::var userVar = stemVar["User"];
                    juce::var referenceVar = stemVar["Reference"];
                    
                    LoudnessValues values;
                    
                    if (userVar.hasProperty("IntegratedLUFS"))
                        values.userIntegratedLUFS = static_cast<float>(userVar["IntegratedLUFS"]);
                    if (userVar.hasProperty("ShortTermLUFS"))
                        values.userShortTermLUFS = static_cast<float>(userVar["ShortTermLUFS"]);
                    if (userVar.hasProperty("MomentaryLUFS"))
                        values.userMomentaryLUFS = static_cast<float>(userVar["MomentaryLUFS"]);
                    if (userVar.hasProperty("RMS"))
                        values.userRMS = static_cast<float>(userVar["RMS"]);
                    
                    if (referenceVar.hasProperty("IntegratedLUFS"))
                        values.referenceIntegratedLUFS = static_cast<float>(referenceVar["IntegratedLUFS"]);
                    if (referenceVar.hasProperty("ShortTermLUFS"))
                        values.referenceShortTermLUFS = static_cast<float>(referenceVar["ShortTermLUFS"]);
                    if (referenceVar.hasProperty("MomentaryLUFS"))
                        values.referenceMomentaryLUFS = static_cast<float>(referenceVar["MomentaryLUFS"]);
                    if (referenceVar.hasProperty("RMS"))
                        values.referenceRMS = static_cast<float>(referenceVar["RMS"]);
                    
                    loudnessValues[stemType] = values;
                }
            }
        }
    }
    
    // Parse width data
    if (parsedJson.hasProperty("Width") && parsedJson["Width"].isObject())
    {
        juce::var widthVar = parsedJson["Width"];
        
        for (int i = 0; i < 6; ++i)
        {
            StemType stemType = static_cast<StemType>(i);
            juce::String stemName;
            
            switch (stemType)
            {
                case StemType::FullMix: stemName = "FullMix"; break;
                case StemType::Kick: stemName = "Kick"; break;
                case StemType::Snare: stemName = "Snare"; break;
                case StemType::Bass: stemName = "Bass"; break;
                case StemType::Vocals: stemName = "Vocals"; break;
                case StemType::Other: stemName = "Other"; break;
                default: stemName = "Unknown"; break;
            }
            
            if (widthVar.hasProperty(stemName) && widthVar[stemName].isObject())
            {
                juce::var stemVar = widthVar[stemName];
                
                if (stemVar.hasProperty("User") && stemVar["User"].isObject() &&
                    stemVar.hasProperty("Reference") && stemVar["Reference"].isObject())
                {
                    juce::var userVar = stemVar["User"];
                    juce::var referenceVar = stemVar["Reference"];
                    
                    WidthValues values;
                    
                    if (userVar.hasProperty("Correlation"))
                        values.userCorrelation = static_cast<float>(userVar["Correlation"]);
                    if (userVar.hasProperty("MidSideRatio"))
                        values.userMidSideRatio = static_cast<float>(userVar["MidSideRatio"]);
                    
                    if (referenceVar.hasProperty("Correlation"))
                        values.referenceCorrelation = static_cast<float>(referenceVar["Correlation"]);
                    if (referenceVar.hasProperty("MidSideRatio"))
                        values.referenceMidSideRatio = static_cast<float>(referenceVar["MidSideRatio"]);
                    
                    widthValues[stemType] = values;
                }
            }
        }
    }
    
    return true;
}

float ComparisonResult::calculateLoudnessMatchScore(float difference, LoudnessAnalyzer::LoudnessType type) const
{
    // Perfect match
    if (difference < loudnessPerfectMatchThreshold)
    {
        return 1.0f;
    }
    // Good match
    else if (difference < loudnessGoodMatchThreshold)
    {
        float t = (difference - loudnessPerfectMatchThreshold) / (loudnessGoodMatchThreshold - loudnessPerfectMatchThreshold);
        return 1.0f - (0.25f * t); // 1.0 -> 0.75
    }
    // Fair match
    else if (difference < loudnessFairMatchThreshold)
    {
        float t = (difference - loudnessGoodMatchThreshold) / (loudnessFairMatchThreshold - loudnessGoodMatchThreshold);
        return 0.75f - (0.25f * t); // 0.75 -> 0.5
    }
    // Poor match
    else
    {
        // Exponential falloff for very poor matches
        return 0.5f * std::exp(-(difference - loudnessFairMatchThreshold) / 5.0f);
    }
}

float ComparisonResult::calculateWidthMatchScore(float difference, StereoWidthAnalyzer::WidthType type) const
{
    // Perfect match
    if (difference < widthPerfectMatchThreshold)
    {
        return 1.0f;
    }
    // Good match
    else if (difference < widthGoodMatchThreshold)
    {
        float t = (difference - widthPerfectMatchThreshold) / (widthGoodMatchThreshold - widthPerfectMatchThreshold);
        return 1.0f - (0.25f * t); // 1.0 -> 0.75
    }
    // Fair match
    else if (difference < widthFairMatchThreshold)
    {
        float t = (difference - widthGoodMatchThreshold) / (widthFairMatchThreshold - widthGoodMatchThreshold);
        return 0.75f - (0.25f * t); // 0.75 -> 0.5
    }
    // Poor match
    else
    {
        // Exponential falloff for very poor matches
        return 0.5f * std::exp(-(difference - widthFairMatchThreshold) / 0.2f);
    }
}

} // namespace ForensEQ
