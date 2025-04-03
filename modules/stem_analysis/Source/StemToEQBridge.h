#pragma once

#include <JuceHeader.h>
#include "StemAnalysisComponent.h"

namespace ForensEQ {

/**
 * Interface class for integrating stem analysis with the EQ visualizer module
 */
class StemToEQBridge : public juce::ChangeListener {
public:
    StemToEQBridge(StemAnalysisComponent& stemAnalysis);
    ~StemToEQBridge();
    
    // Get the frequency data for the currently selected stem
    void getSelectedStemEQData(std::vector<float>& frequencies, std::vector<float>& magnitudes);
    
    // Get the stem type as a string
    juce::String getSelectedStemName() const;
    
    // Check if stem data is available
    bool hasStemData() const;
    
    // Notify when stem selection changes
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void stemSelectionChanged() = 0;
    };
    
    void addListener(Listener* listener);
    void removeListener(Listener* listener);

private:
    StemAnalysisComponent& stemAnalysis;
    juce::ListenerList<Listener> listeners;
    
    // Internal callback for stem changes
    void handleStemChange();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StemToEQBridge)
};

} // namespace ForensEQ
