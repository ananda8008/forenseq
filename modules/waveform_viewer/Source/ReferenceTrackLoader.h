#pragma once

#include <JuceHeader.h>
#include "WaveformViewerComponent.h"

namespace ForensEQ {

/**
 * ReferenceTrackLoader - A component that handles loading and managing reference tracks
 * for the WaveformViewerComponent.
 */
class ReferenceTrackLoader : public juce::Component,
                            public juce::FileDragAndDropTarget,
                            public juce::Button::Listener
{
public:
    ReferenceTrackLoader(WaveformViewerComponent& waveformViewer);
    ~ReferenceTrackLoader() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // FileDragAndDropTarget implementation
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    
    // Button::Listener implementation
    void buttonClicked(juce::Button* button) override;
    
    // Load a reference track from file
    bool loadReferenceTrack(const juce::File& file, int trackIndex);
    
    // Open file chooser to load a reference track
    void openFileChooser(int trackIndex);

private:
    // Reference to the waveform viewer component
    WaveformViewerComponent& waveformViewer;
    
    // Audio format manager
    juce::AudioFormatManager formatManager;
    
    // Load buttons for each track
    juce::TextButton loadButton1{"Load Track 1"};
    juce::TextButton loadButton2{"Load Track 2"};
    juce::TextButton loadButton3{"Load Track 3"};
    
    // Track info labels
    juce::Label trackLabel1;
    juce::Label trackLabel2;
    juce::Label trackLabel3;
    
    // UI colors
    juce::Colour backgroundColor = juce::Colour(40, 40, 40);      // Slightly lighter than main background
    juce::Colour textColor = juce::Colour(220, 220, 220);         // Light gray
    
    // Helper methods
    void updateTrackLabels();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReferenceTrackLoader)
};

} // namespace ForensEQ
