#pragma once

#include <JuceHeader.h>
#include "WaveformViewerComponent.h"
#include "ReferenceTrackLoader.h"
#include "TrackInfoComponent.h"

namespace ForensEQ {

/**
 * TrackSwitcherComponent - A component that provides UI for switching between reference tracks
 */
class TrackSwitcherComponent : public juce::Component,
                              public juce::Button::Listener
{
public:
    TrackSwitcherComponent(WaveformViewerComponent& waveformViewer);
    ~TrackSwitcherComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Button::Listener implementation
    void buttonClicked(juce::Button* button) override;
    
    // Update the display to reflect the current track
    void updateSelectedTrack();

private:
    // Reference to the waveform viewer component
    WaveformViewerComponent& waveformViewer;
    
    // Track selection buttons
    juce::TextButton track1Button{"Track 1"};
    juce::TextButton track2Button{"Track 2"};
    juce::TextButton track3Button{"Track 3"};
    
    // UI colors
    juce::Colour backgroundColor = juce::Colour(40, 40, 40);
    juce::Colour textColor = juce::Colour(220, 220, 220);
    juce::Colour selectedColor = juce::Colour(0, 120, 255);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackSwitcherComponent)
};

} // namespace ForensEQ
