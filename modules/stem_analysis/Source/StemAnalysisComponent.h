#pragma once

#include <JuceHeader.h>
#include "StemManager.h"
#include "StemSelectorComponent.h"
#include "StemInfoComponent.h"

namespace ForensEQ {

/**
 * Main component for the stem analysis module that integrates all UI elements
 */
class StemAnalysisComponent : public juce::Component,
                             public juce::FileDragAndDropTarget
{
public:
    StemAnalysisComponent();
    ~StemAnalysisComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // FileDragAndDropTarget implementation
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    
    // Get the stem manager
    StemManager& getStemManager() { return stemManager; }
    
    // Load a reference track
    bool loadReferenceTrack(const juce::File& file);
    
    // Get the currently selected stem type
    StemType getSelectedStemType() const;
    
    // Get the frequency data for the selected stem
    void getSelectedStemFrequencyData(std::vector<float>& frequencies, std::vector<float>& magnitudes) const;

private:
    // Core components
    StemManager stemManager;
    StemSelectorComponent stemSelector{stemManager};
    StemInfoComponent stemInfo{stemManager};
    
    // UI colors
    juce::Colour backgroundColor = juce::Colour(30, 30, 30);
    juce::Colour textColor = juce::Colour(220, 220, 220);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StemAnalysisComponent)
};

} // namespace ForensEQ
