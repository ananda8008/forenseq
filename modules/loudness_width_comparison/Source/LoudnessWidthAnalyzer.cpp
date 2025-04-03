#include "LoudnessWidthAnalyzer.h"

namespace ForensEQ {

LoudnessWidthAnalyzer::LoudnessWidthAnalyzer()
{
}

LoudnessWidthAnalyzer::~LoudnessWidthAnalyzer()
{
}

ComparisonResult LoudnessWidthAnalyzer::analyzeAndCompare(
    const juce::AudioBuffer<float>& userMix,
    const juce::AudioBuffer<float>& referenceMix,
    const std::map<ComparisonResult::StemType, juce::AudioBuffer<float>>& userStems,
    const std::map<ComparisonResult::StemType, juce::AudioBuffer<float>>& referenceStems,
    double sampleRate)
{
    ComparisonResult result;
    
    // Analyze full mix
    float userIntegratedLUFS, userShortTermLUFS, userMomentaryLUFS, userRMS;
    float userCorrelation, userMidSideRatio;
    
    float refIntegratedLUFS, refShortTermLUFS, refMomentaryLUFS, refRMS;
    float refCorrelation, refMidSideRatio;
    
    // Analyze user mix
    analyzeSingleTrack(
        userMix,
        userIntegratedLUFS,
        userShortTermLUFS,
        userMomentaryLUFS,
        userRMS,
        userCorrelation,
        userMidSideRatio,
        sampleRate
    );
    
    // Analyze reference mix
    analyzeSingleTrack(
        referenceMix,
        refIntegratedLUFS,
        refShortTermLUFS,
        refMomentaryLUFS,
        refRMS,
        refCorrelation,
        refMidSideRatio,
        sampleRate
    );
    
    // Store full mix results
    result.setLoudnessValues(
        ComparisonResult::StemType::FullMix,
        userIntegratedLUFS,
        refIntegratedLUFS,
        userShortTermLUFS,
        refShortTermLUFS,
        userMomentaryLUFS,
        refMomentaryLUFS,
        userRMS,
        refRMS
    );
    
    result.setWidthValues(
        ComparisonResult::StemType::FullMix,
        userCorrelation,
        refCorrelation,
        userMidSideRatio,
        refMidSideRatio
    );
    
    // Analyze each stem
    for (int i = 1; i < 6; ++i) // Skip FullMix (0)
    {
        ComparisonResult::StemType stemType = static_cast<ComparisonResult::StemType>(i);
        
        // Check if both user and reference stems are available
        auto userStemIt = userStems.find(stemType);
        auto refStemIt = referenceStems.find(stemType);
        
        if (userStemIt != userStems.end() && refStemIt != referenceStems.end())
        {
            // Analyze user stem
            analyzeSingleTrack(
                userStemIt->second,
                userIntegratedLUFS,
                userShortTermLUFS,
                userMomentaryLUFS,
                userRMS,
                userCorrelation,
                userMidSideRatio,
                sampleRate
            );
            
            // Analyze reference stem
            analyzeSingleTrack(
                refStemIt->second,
                refIntegratedLUFS,
                refShortTermLUFS,
                refMomentaryLUFS,
                refRMS,
                refCorrelation,
                refMidSideRatio,
                sampleRate
            );
            
            // Store stem results
            result.setLoudnessValues(
                stemType,
                userIntegratedLUFS,
                refIntegratedLUFS,
                userShortTermLUFS,
                refShortTermLUFS,
                userMomentaryLUFS,
                refMomentaryLUFS,
                userRMS,
                refRMS
            );
            
            result.setWidthValues(
                stemType,
                userCorrelation,
                refCorrelation,
                userMidSideRatio,
                refMidSideRatio
            );
        }
    }
    
    return result;
}

void LoudnessWidthAnalyzer::analyzeSingleTrack(
    const juce::AudioBuffer<float>& buffer,
    float& integratedLUFS,
    float& shortTermLUFS,
    float& momentaryLUFS,
    float& rms,
    float& correlation,
    float& midSideRatio,
    double sampleRate)
{
    // Calculate loudness metrics
    integratedLUFS = loudnessAnalyzer.calculateLoudness(
        buffer, 
        LoudnessAnalyzer::LoudnessType::Integrated, 
        sampleRate
    );
    
    shortTermLUFS = loudnessAnalyzer.calculateLoudness(
        buffer, 
        LoudnessAnalyzer::LoudnessType::ShortTerm, 
        sampleRate
    );
    
    momentaryLUFS = loudnessAnalyzer.calculateLoudness(
        buffer, 
        LoudnessAnalyzer::LoudnessType::Momentary, 
        sampleRate
    );
    
    rms = loudnessAnalyzer.calculateLoudness(
        buffer, 
        LoudnessAnalyzer::LoudnessType::RMS, 
        sampleRate
    );
    
    // Calculate width metrics
    correlation = stereoWidthAnalyzer.calculateWidth(
        buffer, 
        StereoWidthAnalyzer::WidthType::Correlation
    );
    
    midSideRatio = stereoWidthAnalyzer.calculateWidth(
        buffer, 
        StereoWidthAnalyzer::WidthType::MidSideRatio
    );
}

} // namespace ForensEQ
