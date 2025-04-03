#include "LoudnessToEQBridge.h"

namespace ForensEQ {

LoudnessToEQBridge::LoudnessToEQBridge()
{
}

LoudnessToEQBridge::~LoudnessToEQBridge()
{
}

void LoudnessToEQBridge::setComparisonResult(const ComparisonResult& result)
{
    currentResult = result;
}

void LoudnessToEQBridge::setStemType(ComparisonResult::StemType type)
{
    currentStemType = type;
}

juce::Array<float> LoudnessToEQBridge::getEQFrequencyData(bool isUserMix) const
{
    return generateEQDataFromLoudness(isUserMix);
}

float LoudnessToEQBridge::getEQMatchPercentage() const
{
    return currentResult.getCombinedMatchScore(currentStemType);
}

juce::Array<float> LoudnessToEQBridge::generateEQDataFromLoudness(bool isUserMix) const
{
    // Create an array to hold frequency data for the EQ visualizer
    // This is a simplified approach - in a real implementation, this would
    // use actual frequency analysis data from the audio
    
    // Create frequency bands (typically 31-band EQ)
    const int numBands = 31;
    juce::Array<float> eqData;
    eqData.resize(numBands);
    
    // Define frequency bands (in Hz)
    const float bands[numBands] = {
        20, 25, 31.5, 40, 50, 63, 80, 100, 125, 160, 200, 250, 315, 400, 500,
        630, 800, 1000, 1250, 1600, 2000, 2500, 3150, 4000, 5000, 6300, 8000,
        10000, 12500, 16000, 20000
    };
    
    // Get loudness values for the current stem
    float integratedLUFS = isUserMix 
        ? currentResult.getUserLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::Integrated)
        : currentResult.getReferenceLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::Integrated);
    
    float shortTermLUFS = isUserMix 
        ? currentResult.getUserLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::ShortTerm)
        : currentResult.getReferenceLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::ShortTerm);
    
    float momentaryLUFS = isUserMix 
        ? currentResult.getUserLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::Momentary)
        : currentResult.getReferenceLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::Momentary);
    
    float rms = isUserMix 
        ? currentResult.getUserLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::RMS)
        : currentResult.getReferenceLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::RMS);
    
    // Get width values for the current stem
    float correlation = isUserMix 
        ? currentResult.getUserWidth(currentStemType, StereoWidthAnalyzer::WidthType::Correlation)
        : currentResult.getReferenceWidth(currentStemType, StereoWidthAnalyzer::WidthType::Correlation);
    
    float widthPercentage = isUserMix 
        ? currentResult.getUserWidth(currentStemType, StereoWidthAnalyzer::WidthType::Percentage)
        : currentResult.getReferenceWidth(currentStemType, StereoWidthAnalyzer::WidthType::Percentage);
    
    // Generate EQ curve based on stem type and loudness/width values
    // This is a simplified approach that creates a plausible EQ curve
    // In a real implementation, this would use actual frequency analysis
    
    // Base level derived from integrated LUFS
    float baseLevel = (integratedLUFS + 70.0f) / 70.0f; // Normalize to 0-1 range
    
    // Apply stem-specific EQ curves
    switch (currentStemType)
    {
        case ComparisonResult::StemType::Kick:
            // Emphasize low frequencies for kick
            for (int i = 0; i < numBands; ++i)
            {
                if (bands[i] < 100)
                    eqData.set(i, baseLevel * 1.5f * (1.0f - bands[i] / 100.0f));
                else if (bands[i] < 500)
                    eqData.set(i, baseLevel * 0.8f * (1.0f - (bands[i] - 100.0f) / 400.0f));
                else
                    eqData.set(i, baseLevel * 0.2f);
            }
            break;
            
        case ComparisonResult::StemType::Snare:
            // Emphasize mid-low frequencies for snare
            for (int i = 0; i < numBands; ++i)
            {
                if (bands[i] > 100 && bands[i] < 500)
                    eqData.set(i, baseLevel * 1.3f);
                else if (bands[i] > 2000 && bands[i] < 8000)
                    eqData.set(i, baseLevel * 1.1f);
                else
                    eqData.set(i, baseLevel * 0.7f);
            }
            break;
            
        case ComparisonResult::StemType::Bass:
            // Emphasize low and low-mid frequencies for bass
            for (int i = 0; i < numBands; ++i)
            {
                if (bands[i] < 250)
                    eqData.set(i, baseLevel * 1.4f);
                else if (bands[i] < 1000)
                    eqData.set(i, baseLevel * 1.0f);
                else
                    eqData.set(i, baseLevel * 0.5f * (1.0f - (bands[i] - 1000.0f) / 19000.0f));
            }
            break;
            
        case ComparisonResult::StemType::Vocals:
            // Emphasize mid frequencies for vocals
            for (int i = 0; i < numBands; ++i)
            {
                if (bands[i] > 250 && bands[i] < 4000)
                    eqData.set(i, baseLevel * 1.3f);
                else
                    eqData.set(i, baseLevel * 0.7f);
            }
            break;
            
        case ComparisonResult::StemType::Other:
            // Varied frequency response for other elements
            for (int i = 0; i < numBands; ++i)
            {
                if (bands[i] > 2000)
                    eqData.set(i, baseLevel * 1.2f);
                else
                    eqData.set(i, baseLevel * 0.8f);
            }
            break;
            
        case ComparisonResult::StemType::FullMix:
        default:
            // Full range for full mix
            for (int i = 0; i < numBands; ++i)
            {
                // Create a gentle curve with slight emphasis on low-mid and high frequencies
                float factor = 1.0f;
                
                // Boost bass slightly
                if (bands[i] < 250)
                    factor = 1.1f;
                
                // Slight dip in low-mids
                else if (bands[i] < 500)
                    factor = 0.9f;
                
                // Boost presence
                else if (bands[i] > 2000 && bands[i] < 8000)
                    factor = 1.1f;
                
                // Boost air
                else if (bands[i] > 10000)
                    factor = 1.05f;
                
                eqData.set(i, baseLevel * factor);
            }
            break;
    }
    
    // Apply width influence - wider mixes typically have more high frequency content
    for (int i = 0; i < numBands; ++i)
    {
        if (bands[i] > 5000)
        {
            float widthFactor = widthPercentage / 50.0f; // Normalize to 1.0 at 50% width
            eqData.set(i, eqData[i] * (0.8f + 0.4f * widthFactor));
        }
    }
    
    // Apply some randomness to make it look more natural
    juce::Random random;
    for (int i = 0; i < numBands; ++i)
    {
        float randomFactor = 0.9f + 0.2f * random.nextFloat();
        eqData.set(i, eqData[i] * randomFactor);
    }
    
    // Smooth the curve
    for (int i = 1; i < numBands - 1; ++i)
    {
        eqData.set(i, (eqData[i-1] + eqData[i] * 2.0f + eqData[i+1]) / 4.0f);
    }
    
    return eqData;
}

} // namespace ForensEQ
