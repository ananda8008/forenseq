#pragma once

#include <JuceHeader.h>
#include "WaveformViewerComponent.h"
#include "ReferenceTrackLoader.h"

namespace ForensEQ {

/**
 * TrackInfoComponent - A component that displays information about the currently selected reference track
 * and provides controls for level matching and time alignment.
 */
class TrackInfoComponent : public juce::Component,
                          public juce::Slider::Listener
{
public:
    TrackInfoComponent(WaveformViewerComponent& waveformViewer);
    ~TrackInfoComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Slider::Listener implementation
    void sliderValueChanged(juce::Slider* slider) override;
    
    // Update the display with current track information
    void updateTrackInfo();

private:
    // Reference to the waveform viewer component
    WaveformViewerComponent& waveformViewer;
    
    // Track information labels
    juce::Label trackNameLabel;
    juce::Label trackInfoLabel;
    
    // Level matching controls
    juce::Label levelLabel{"", "Level:"};
    juce::Slider levelSlider;
    
    // Time alignment controls
    juce::Label timeAlignLabel{"", "Time Offset:"};
    juce::Slider timeAlignSlider;
    
    // UI colors
    juce::Colour backgroundColor = juce::Colour(40, 40, 40);
    juce::Colour textColor = juce::Colour(220, 220, 220);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackInfoComponent)
};

} // namespace ForensEQ
