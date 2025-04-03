#include "TrackSwitcherComponent.h"

namespace ForensEQ {

TrackSwitcherComponent::TrackSwitcherComponent(WaveformViewerComponent& waveformViewer)
    : waveformViewer(waveformViewer)
{
    // Set up track buttons
    addAndMakeVisible(track1Button);
    addAndMakeVisible(track2Button);
    addAndMakeVisible(track3Button);
    
    track1Button.addListener(this);
    track2Button.addListener(this);
    track3Button.addListener(this);
    
    // Style the buttons
    for (auto* button : {&track1Button, &track2Button, &track3Button})
    {
        button->setColour(juce::TextButton::buttonColourId, juce::Colour(60, 60, 60));
        button->setColour(juce::TextButton::textColourOffId, textColor);
    }
    
    // Update to show current track
    updateSelectedTrack();
}

TrackSwitcherComponent::~TrackSwitcherComponent()
{
    track1Button.removeListener(this);
    track2Button.removeListener(this);
    track3Button.removeListener(this);
}

void TrackSwitcherComponent::paint(juce::Graphics& g)
{
    g.fillAll(backgroundColor);
}

void TrackSwitcherComponent::resized()
{
    auto bounds = getLocalBounds();
    
    // Divide the width equally among the three buttons
    int buttonWidth = bounds.getWidth() / 3;
    
    track1Button.setBounds(bounds.removeFromLeft(buttonWidth));
    track2Button.setBounds(bounds.removeFromLeft(buttonWidth));
    track3Button.setBounds(bounds);
}

void TrackSwitcherComponent::buttonClicked(juce::Button* button)
{
    int trackIndex = -1;
    
    if (button == &track1Button)
        trackIndex = 0;
    else if (button == &track2Button)
        trackIndex = 1;
    else if (button == &track3Button)
        trackIndex = 2;
    
    if (trackIndex >= 0)
    {
        waveformViewer.switchToReferenceTrack(trackIndex);
        updateSelectedTrack();
    }
}

void TrackSwitcherComponent::updateSelectedTrack()
{
    int currentTrack = waveformViewer.getCurrentReferenceTrackIndex();
    
    // Reset all buttons
    track1Button.setColour(juce::TextButton::buttonColourId, juce::Colour(60, 60, 60));
    track2Button.setColour(juce::TextButton::buttonColourId, juce::Colour(60, 60, 60));
    track3Button.setColour(juce::TextButton::buttonColourId, juce::Colour(60, 60, 60));
    
    // Highlight the selected track
    switch (currentTrack)
    {
        case 0:
            track1Button.setColour(juce::TextButton::buttonColourId, selectedColor);
            break;
        case 1:
            track2Button.setColour(juce::TextButton::buttonColourId, selectedColor);
            break;
        case 2:
            track3Button.setColour(juce::TextButton::buttonColourId, selectedColor);
            break;
    }
    
    // Update button enabled state based on whether tracks are loaded
    int numTracks = waveformViewer.getNumReferenceTracks();
    
    track1Button.setEnabled(numTracks > 0);
    track2Button.setEnabled(numTracks > 1);
    track3Button.setEnabled(numTracks > 2);
    
    repaint();
}

} // namespace ForensEQ
