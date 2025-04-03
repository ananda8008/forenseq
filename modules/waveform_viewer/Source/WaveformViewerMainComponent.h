#pragma once

#include <JuceHeader.h>
#include "WaveformViewerComponent.h"
#include "ReferenceTrackLoader.h"
#include "TrackInfoComponent.h"
#include "TrackSwitcherComponent.h"

namespace ForensEQ {

/**
 * WaveformViewerMainComponent - A component that integrates all the waveform viewer components
 * into a single cohesive interface.
 */
class WaveformViewerMainComponent : public juce::Component
{
public:
    WaveformViewerMainComponent();
    ~WaveformViewerMainComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    // Core components
    WaveformViewerComponent waveformViewer;
    ReferenceTrackLoader trackLoader{waveformViewer};
    TrackInfoComponent trackInfo{waveformViewer};
    TrackSwitcherComponent trackSwitcher{waveformViewer};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformViewerMainComponent)
};

} // namespace ForensEQ
