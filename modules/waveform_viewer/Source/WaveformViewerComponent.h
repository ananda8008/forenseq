#pragma once

#include <JuceHeader.h>

namespace ForensEQ {

// Forward declaration
class WaveformDisplay;

/**
 * WaveformViewerComponent - A component that displays waveforms of reference audio tracks
 * and allows for interaction with them.
 */
class WaveformViewerComponent : public juce::Component,
                               public juce::FileDragAndDropTarget,
                               public juce::Timer
{
public:
    WaveformViewerComponent();
    ~WaveformViewerComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

    // FileDragAndDropTarget implementation
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    // Load a reference track from file
    bool loadReferenceTrack(const juce::File& file, int trackIndex = 0);
    
    // Get the number of loaded reference tracks
    int getNumReferenceTracks() const;
    
    // Switch to a specific reference track
    void switchToReferenceTrack(int trackIndex);
    
    // Get the currently active reference track index
    int getCurrentReferenceTrackIndex() const;
    
    // Mouse interaction
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

private:
    // Maximum number of reference tracks
    static constexpr int maxReferenceTracks = 3;
    
    // Reference track data
    struct ReferenceTrack
    {
        juce::File file;
        std::unique_ptr<juce::AudioFormatReader> reader;
        juce::AudioThumbnail thumbnail;
        juce::String name;
        double lengthInSeconds = 0.0;
        bool loaded = false;
        
        ReferenceTrack(juce::AudioThumbnailCache& cache, juce::AudioFormatManager& formatManager)
            : thumbnail(512, formatManager, cache) {}
    };
    
    // Audio format manager and cache
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbnailCache;
    
    // Reference tracks
    std::array<ReferenceTrack, maxReferenceTracks> referenceTracks;
    int currentTrackIndex = 0;
    
    // Selection and playback
    double currentPosition = 0.0;
    double selectionStart = 0.0;
    double selectionEnd = 0.0;
    bool isSelecting = false;
    
    // UI colors
    juce::Colour backgroundColor = juce::Colour(30, 30, 30);      // Dark charcoal
    juce::Colour waveformColor = juce::Colour(0, 120, 255);       // Blue
    juce::Colour playPositionColor = juce::Colour(255, 255, 200); // Light gold
    juce::Colour selectionColor = juce::Colour(255, 120, 0);      // Orange
    juce::Colour textColor = juce::Colour(220, 220, 220);         // Light gray
    
    // Helper methods
    double pixelToTime(int x, juce::Rectangle<int> bounds) const;
    int timeToPixel(double time, juce::Rectangle<int> bounds) const;
    
    // Additional methods for integration with other components
    juce::String getTrackName(int trackIndex) const;
    double getTrackDuration(int trackIndex) const;
    double getTrackSampleRate(int trackIndex) const;
    int getTrackNumChannels(int trackIndex) const;
    WaveformDisplay* getWaveformDisplay();
    
    // Waveform display component
    std::unique_ptr<WaveformDisplay> waveformDisplay;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformViewerComponent)
};

} // namespace ForensEQ
