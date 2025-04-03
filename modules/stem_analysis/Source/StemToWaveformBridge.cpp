#include "StemToWaveformBridge.h"

namespace ForensEQ {

StemToWaveformBridge::StemToWaveformBridge(StemAnalysisComponent& stemAnalysis)
    : stemAnalysis(stemAnalysis)
{
    // Register as a change listener with the stem manager
    stemAnalysis.getStemManager().addChangeListener(this);
}

StemToWaveformBridge::~StemToWaveformBridge()
{
    // Remove change listener
    stemAnalysis.getStemManager().removeChangeListener(this);
}

void StemToWaveformBridge::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &stemAnalysis.getStemManager())
    {
        // Handle stem change
        handleStemChange();
    }
}

const juce::AudioBuffer<float>* StemToWaveformBridge::getSelectedStemAudio() const
{
    // Get the selected stem type
    StemType type = stemAnalysis.getSelectedStemType();
    
    // Get the stem data
    StemData* stem = stemAnalysis.getStemManager().getStem(type);
    
    if (stem != nullptr && stem->hasValidAudio())
    {
        return &stem->getAudioBuffer();
    }
    
    return nullptr;
}

juce::String StemToWaveformBridge::getSelectedStemName() const
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

bool StemToWaveformBridge::hasStemAudio() const
{
    // Get the selected stem type
    StemType type = stemAnalysis.getSelectedStemType();
    
    // Get the stem data
    StemData* stem = stemAnalysis.getStemManager().getStem(type);
    
    return (stem != nullptr && stem->hasValidAudio());
}

void StemToWaveformBridge::addListener(Listener* listener)
{
    listeners.add(listener);
}

void StemToWaveformBridge::removeListener(Listener* listener)
{
    listeners.remove(listener);
}

void StemToWaveformBridge::handleStemChange()
{
    // Notify all listeners that the stem audio has changed
    listeners.call(&Listener::stemAudioChanged);
}

} // namespace ForensEQ
