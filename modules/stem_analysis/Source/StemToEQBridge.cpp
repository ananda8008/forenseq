#include "StemToEQBridge.h"

namespace ForensEQ {

StemToEQBridge::StemToEQBridge(StemAnalysisComponent& stemAnalysis)
    : stemAnalysis(stemAnalysis)
{
    // Register as a change listener with the stem manager
    stemAnalysis.getStemManager().addChangeListener(this);
}

void StemToEQBridge::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &stemAnalysis.getStemManager())
    {
        // Handle stem change
        handleStemChange();
    }
}

StemToEQBridge::~StemToEQBridge()
{
    // Remove change listener
    stemAnalysis.getStemManager().removeChangeListener(this);
}

void StemToEQBridge::getSelectedStemEQData(std::vector<float>& frequencies, std::vector<float>& magnitudes)
{
    // Get the frequency data from the selected stem
    stemAnalysis.getSelectedStemFrequencyData(frequencies, magnitudes);
}

juce::String StemToEQBridge::getSelectedStemName() const
{
    // Get the selected stem type
    StemType type = stemAnalysis.getSelectedStemType();
    
    // Convert to string
    switch (type)
    {
        case StemType::Kick:
            return "Kick";
        case StemType::Snare:
            return "Snare";
        case StemType::Bass:
            return "Bass";
        case StemType::Vocals:
            return "Vocals";
        case StemType::Other:
            return "Other";
        case StemType::Full:
        default:
            return "Full Mix";
    }
}

bool StemToEQBridge::hasStemData() const
{
    // Check if the selected stem has valid frequency data
    std::vector<float> frequencies, magnitudes;
    stemAnalysis.getSelectedStemFrequencyData(frequencies, magnitudes);
    
    return !frequencies.empty() && !magnitudes.empty();
}

void StemToEQBridge::addListener(Listener* listener)
{
    listeners.add(listener);
}

void StemToEQBridge::removeListener(Listener* listener)
{
    listeners.remove(listener);
}

void StemToEQBridge::handleStemChange()
{
    // Notify all listeners that the stem selection has changed
    listeners.call(&Listener::stemSelectionChanged);
}

} // namespace ForensEQ
