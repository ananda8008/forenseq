#include "WaveformViewerMainComponent.h"

namespace ForensEQ {

WaveformViewerMainComponent::WaveformViewerMainComponent()
{
    // Add all components
    addAndMakeVisible(waveformViewer);
    addAndMakeVisible(trackLoader);
    addAndMakeVisible(trackInfo);
    addAndMakeVisible(trackSwitcher);
    
    // Set size
    setSize(800, 600);
}

WaveformViewerMainComponent::~WaveformViewerMainComponent()
{
}

void WaveformViewerMainComponent::paint(juce::Graphics& g)
{
    // Fill background
    g.fillAll(juce::Colour(30, 30, 30));
}

void WaveformViewerMainComponent::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
    // Position the track switcher at the top
    trackSwitcher.setBounds(bounds.removeFromTop(30));
    
    bounds.removeFromTop(10); // Spacing
    
    // Position the track loader on the left side
    trackLoader.setBounds(bounds.removeFromLeft(200).removeFromTop(150));
    
    // Position the waveform viewer in the main area
    auto waveformBounds = bounds;
    waveformBounds.removeFromBottom(80); // Space for track info
    waveformViewer.setBounds(waveformBounds);
    
    // Position the track info at the bottom
    trackInfo.setBounds(bounds.removeFromBottom(70));
}

} // namespace ForensEQ
