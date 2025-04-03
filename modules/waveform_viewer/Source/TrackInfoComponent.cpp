#include "TrackInfoComponent.h"

namespace ForensEQ {

TrackInfoComponent::TrackInfoComponent(WaveformViewerComponent& waveformViewer)
    : waveformViewer(waveformViewer)
{
    // Set up track name label
    trackNameLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    trackNameLabel.setColour(juce::Label::textColourId, textColor);
    trackNameLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(trackNameLabel);
    
    // Set up track info label
    trackInfoLabel.setFont(juce::Font(14.0f));
    trackInfoLabel.setColour(juce::Label::textColourId, textColor);
    trackInfoLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(trackInfoLabel);
    
    // Set up level matching controls
    levelLabel.setFont(juce::Font(14.0f));
    levelLabel.setColour(juce::Label::textColourId, textColor);
    levelLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(levelLabel);
    
    levelSlider.setRange(-24.0, 24.0, 0.1);
    levelSlider.setValue(0.0);
    levelSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    levelSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0, 120, 255));
    levelSlider.setColour(juce::Slider::trackColourId, juce::Colour(60, 60, 60));
    levelSlider.setColour(juce::Slider::textBoxTextColourId, textColor);
    levelSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    levelSlider.addListener(this);
    addAndMakeVisible(levelSlider);
    
    // Set up time alignment controls
    timeAlignLabel.setFont(juce::Font(14.0f));
    timeAlignLabel.setColour(juce::Label::textColourId, textColor);
    timeAlignLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(timeAlignLabel);
    
    timeAlignSlider.setRange(-5.0, 5.0, 0.01);
    timeAlignSlider.setValue(0.0);
    timeAlignSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    timeAlignSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0, 120, 255));
    timeAlignSlider.setColour(juce::Slider::trackColourId, juce::Colour(60, 60, 60));
    timeAlignSlider.setColour(juce::Slider::textBoxTextColourId, textColor);
    timeAlignSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    timeAlignSlider.addListener(this);
    addAndMakeVisible(timeAlignSlider);
    
    // Initial update
    updateTrackInfo();
}

TrackInfoComponent::~TrackInfoComponent()
{
    levelSlider.removeListener(this);
    timeAlignSlider.removeListener(this);
}

void TrackInfoComponent::paint(juce::Graphics& g)
{
    g.fillAll(backgroundColor);
    
    // Draw separator line
    g.setColour(textColor.withAlpha(0.3f));
    g.drawLine(0, 0, getWidth(), 0, 1.0f);
    g.drawLine(0, getHeight() - 1, getWidth(), getHeight() - 1, 1.0f);
}

void TrackInfoComponent::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
    // Track name label
    trackNameLabel.setBounds(bounds.removeFromTop(25));
    
    bounds.removeFromTop(5); // Spacing
    
    // Track info label
    trackInfoLabel.setBounds(bounds.removeFromTop(20));
    
    bounds.removeFromTop(10); // Spacing
    
    // Level matching controls
    auto levelBounds = bounds.removeFromTop(25);
    levelLabel.setBounds(levelBounds.removeFromLeft(100));
    levelSlider.setBounds(levelBounds);
    
    bounds.removeFromTop(10); // Spacing
    
    // Time alignment controls
    auto timeBounds = bounds.removeFromTop(25);
    timeAlignLabel.setBounds(timeBounds.removeFromLeft(100));
    timeAlignSlider.setBounds(timeBounds);
}

void TrackInfoComponent::sliderValueChanged(juce::Slider* slider)
{
    // Handle slider value changes
    // This would be connected to actual functionality in a full implementation
    if (slider == &levelSlider)
    {
        // Level matching logic would go here
        // For now, just update the display
        updateTrackInfo();
    }
    else if (slider == &timeAlignSlider)
    {
        // Time alignment logic would go here
        // For now, just update the display
        updateTrackInfo();
    }
}

void TrackInfoComponent::updateTrackInfo()
{
    // Get the current track index
    int currentTrackIndex = waveformViewer.getCurrentReferenceTrackIndex();
    
    // Check if there are any tracks loaded
    if (waveformViewer.getNumReferenceTracks() > 0)
    {
        // Set track name (this would need to be implemented in WaveformViewerComponent)
        trackNameLabel.setText("Reference Track " + juce::String(currentTrackIndex + 1), 
                              juce::dontSendNotification);
        
        // Set track info (sample rate, duration, etc.)
        // This would be retrieved from the WaveformViewerComponent in a full implementation
        trackInfoLabel.setText("44.1 kHz, Stereo, 3:45", juce::dontSendNotification);
        
        // Enable controls
        levelSlider.setEnabled(true);
        timeAlignSlider.setEnabled(true);
    }
    else
    {
        // No track loaded
        trackNameLabel.setText("No Reference Track Loaded", juce::dontSendNotification);
        trackInfoLabel.setText("Load a reference track to see information", juce::dontSendNotification);
        
        // Disable controls
        levelSlider.setEnabled(false);
        timeAlignSlider.setEnabled(false);
    }
}

} // namespace ForensEQ
