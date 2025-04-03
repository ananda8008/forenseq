#include "LoudnessToWaveformBridge.h"

namespace ForensEQ {

LoudnessToWaveformBridge::LoudnessToWaveformBridge()
{
    // Generate initial mock data
    generateMockData();
}

LoudnessToWaveformBridge::~LoudnessToWaveformBridge()
{
}

void LoudnessToWaveformBridge::setComparisonResult(const ComparisonResult& result)
{
    currentResult = result;
    
    // Update mock data based on new comparison result
    generateMockData();
}

void LoudnessToWaveformBridge::setStemType(ComparisonResult::StemType type)
{
    currentStemType = type;
    
    // Update mock data based on new stem type
    generateMockData();
}

juce::Array<LoudnessToWaveformBridge::LoudnessMarker> LoudnessToWaveformBridge::getLoudnessMarkers() const
{
    return mockLoudnessMarkers;
}

juce::Array<LoudnessToWaveformBridge::WidthMarker> LoudnessToWaveformBridge::getWidthMarkers() const
{
    return mockWidthMarkers;
}

juce::Array<LoudnessToWaveformBridge::DifferenceRegion> LoudnessToWaveformBridge::getDifferenceRegions() const
{
    return mockDifferenceRegions;
}

void LoudnessToWaveformBridge::generateMockData()
{
    // Clear existing mock data
    mockLoudnessMarkers.clear();
    mockWidthMarkers.clear();
    mockDifferenceRegions.clear();
    
    // Get loudness values for the current stem
    float userIntegratedLUFS = currentResult.getUserLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::Integrated);
    float referenceIntegratedLUFS = currentResult.getReferenceLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::Integrated);
    
    float userShortTermLUFS = currentResult.getUserLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::ShortTerm);
    float referenceShortTermLUFS = currentResult.getReferenceLoudness(currentStemType, LoudnessAnalyzer::LoudnessType::ShortTerm);
    
    // Get width values for the current stem
    float userWidthPercentage = currentResult.getUserWidth(currentStemType, StereoWidthAnalyzer::WidthType::Percentage);
    float referenceWidthPercentage = currentResult.getReferenceWidth(currentStemType, StereoWidthAnalyzer::WidthType::Percentage);
    
    // Create mock loudness markers
    // In a real implementation, these would be derived from actual time-based analysis
    juce::Random random;
    
    // Create markers for a 3-minute track
    const float trackLength = 180.0f; // 3 minutes in seconds
    const int numMarkers = 20;
    
    for (int i = 0; i < numMarkers; ++i)
    {
        float timePosition = (i / (float)(numMarkers - 1)) * trackLength;
        
        // Create user mix marker with some variation around the integrated LUFS
        LoudnessMarker userMarker;
        userMarker.timePosition = timePosition;
        userMarker.loudnessValue = userIntegratedLUFS + random.nextFloat() * 4.0f - 2.0f;
        userMarker.isUserMix = true;
        mockLoudnessMarkers.add(userMarker);
        
        // Create reference mix marker with some variation around the integrated LUFS
        LoudnessMarker referenceMarker;
        referenceMarker.timePosition = timePosition;
        referenceMarker.loudnessValue = referenceIntegratedLUFS + random.nextFloat() * 4.0f - 2.0f;
        referenceMarker.isUserMix = false;
        mockLoudnessMarkers.add(referenceMarker);
        
        // Create width markers
        WidthMarker userWidthMarker;
        userWidthMarker.timePosition = timePosition;
        userWidthMarker.widthValue = userWidthPercentage + random.nextFloat() * 20.0f - 10.0f;
        userWidthMarker.isUserMix = true;
        mockWidthMarkers.add(userWidthMarker);
        
        WidthMarker referenceWidthMarker;
        referenceWidthMarker.timePosition = timePosition;
        referenceWidthMarker.widthValue = referenceWidthPercentage + random.nextFloat() * 20.0f - 10.0f;
        referenceWidthMarker.isUserMix = false;
        mockWidthMarkers.add(referenceWidthMarker);
    }
    
    // Create mock difference regions
    // In a real implementation, these would be derived from actual analysis
    
    // Create a few loudness difference regions
    const int numLoudnessDiffRegions = 3;
    for (int i = 0; i < numLoudnessDiffRegions; ++i)
    {
        DifferenceRegion region;
        region.startTime = random.nextFloat() * (trackLength - 20.0f);
        region.endTime = region.startTime + 10.0f + random.nextFloat() * 10.0f;
        region.difference = std::abs(userIntegratedLUFS - referenceIntegratedLUFS) + random.nextFloat() * 2.0f;
        region.isLoudness = true;
        mockDifferenceRegions.add(region);
    }
    
    // Create a few width difference regions
    const int numWidthDiffRegions = 2;
    for (int i = 0; i < numWidthDiffRegions; ++i)
    {
        DifferenceRegion region;
        region.startTime = random.nextFloat() * (trackLength - 20.0f);
        region.endTime = region.startTime + 10.0f + random.nextFloat() * 10.0f;
        region.difference = std::abs(userWidthPercentage - referenceWidthPercentage) + random.nextFloat() * 10.0f;
        region.isLoudness = false;
        mockDifferenceRegions.add(region);
    }
}

} // namespace ForensEQ
