#pragma once

#include <JuceHeader.h>
#include "StemAnalysisComponent.h"

namespace ForensEQ {

/**
 * Interface class for integrating stem analysis with the waveform viewer module
 */
class StemToWaveformBridge : public juce::ChangeListener {
public:
    StemToWaveformBridge(StemAnalysisComponent& stemAnalysis);
    ~StemToWaveformBridge() override;
    
    // ChangeListener implementation
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    // Get the audio buffer for the currently selected stem
    const juce::AudioBuffer<float>* getSelectedStemAudio() const;
    
    // Get the stem type as a string
    juce::String getSelectedStemName() const;
    
    // Check if stem audio data is available
    bool hasStemAudio() const;
    
    // Notify when stem selection changes
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void stemAudioChanged() = 0;
    };
    
    void addListener(Listener* listener);
    void removeListener(Listener* listener);

private:
    StemAnalysisComponent& stemAnalysis;
    juce::ListenerList<Listener> listeners;
    
    // Internal callback for stem changes
    void handleStemChange();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StemToWaveformBridge)
};

} // namespace ForensEQ
