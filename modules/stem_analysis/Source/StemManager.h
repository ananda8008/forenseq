#pragma once

#include <JuceHeader.h>
#include "StemData.h"
#include "StemIsolator.h"
#include "StemAnalyzer.h"

namespace ForensEQ {

/**
 * Class for managing stem isolation and analysis
 */
class StemManager : public juce::ChangeBroadcaster {
public:
    StemManager();
    ~StemManager();
    
    // Load and process a reference track
    bool loadReferenceTrack(const juce::File& audioFile);
    
    // Get the current reference track file
    juce::File getReferenceTrackFile() const;
    
    // Get a specific stem
    StemData* getStem(StemType type);
    
    // Get the current active stem type
    StemType getActiveStemType() const;
    
    // Set the active stem type
    void setActiveStemType(StemType type);
    
    // Get all available stem types
    std::vector<StemType> getAvailableStemTypes() const;
    
    // Check if a specific stem type is available
    bool isStemAvailable(StemType type) const;
    
    // Get the stem isolator being used
    StemIsolator* getStemIsolator();
    
    // Get the stem analyzer
    StemAnalyzer* getStemAnalyzer();
    
    // Compare the active stem with a reference stem
    float compareWithReference(StemType referenceType);
    
    // Get the number of loaded stems
    int getNumLoadedStems() const;
    
    // Clear all stems
    void clearStems();

private:
    juce::File referenceTrackFile;
    std::map<StemType, std::unique_ptr<StemData>> stems;
    StemType activeStemType = StemType::Full;
    std::unique_ptr<StemIsolator> stemIsolator;
    StemAnalyzer stemAnalyzer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StemManager)
};

} // namespace ForensEQ
