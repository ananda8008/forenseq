#pragma once

#include <JuceHeader.h>

namespace ForensEQ {

/**
 * WaveformDisplay - A component that renders a detailed waveform visualization
 * with customizable appearance and rendering options.
 */
class WaveformDisplay : public juce::Component,
                       public juce::ChangeListener
{
public:
    WaveformDisplay();
    ~WaveformDisplay() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // ChangeListener implementation for thumbnail updates
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    // Set the audio thumbnail to display
    void setAudioThumbnail(juce::AudioThumbnail* thumbnail);
    
    // Set the visible time range
    void setTimeRange(double startTimeSeconds, double endTimeSeconds);
    
    // Set the playback position
    void setPlaybackPosition(double positionSeconds);
    
    // Set the selection range
    void setSelectionRange(double startTimeSeconds, double endTimeSeconds);
    
    // Set the waveform color
    void setWaveformColor(juce::Colour color);
    
    // Set the background color
    void setBackgroundColor(juce::Colour color);
    
    // Set the playback position color
    void setPlaybackPositionColor(juce::Colour color);
    
    // Set the selection color
    void setSelectionColor(juce::Colour color);
    
    // Set whether to show time markers
    void setShowTimeMarkers(bool show);
    
    // Set whether to show stereo channels separately
    void setShowStereoChannels(bool show);
    
    // Set the vertical zoom factor
    void setVerticalZoom(float zoomFactor);

private:
    // The audio thumbnail to display
    juce::AudioThumbnail* audioThumbnail = nullptr;
    
    // Time range
    double startTime = 0.0;
    double endTime = 10.0;
    
    // Playback position
    double playbackPosition = 0.0;
    
    // Selection range
    double selectionStart = 0.0;
    double selectionEnd = 0.0;
    
    // Display options
    bool showTimeMarkers = true;
    bool showStereoChannels = false;
    float verticalZoom = 1.0f;
    
    // Colors
    juce::Colour waveformColor = juce::Colour(0, 120, 255);       // Blue
    juce::Colour backgroundColor = juce::Colour(30, 30, 30);      // Dark charcoal
    juce::Colour playPositionColor = juce::Colour(255, 255, 200); // Light gold
    juce::Colour selectionColor = juce::Colour(255, 120, 0);      // Orange
    juce::Colour textColor = juce::Colour(220, 220, 220);         // Light gray
    
    // Helper methods
    void drawWaveform(juce::Graphics& g, juce::Rectangle<int> bounds);
    void drawPlaybackPosition(juce::Graphics& g, juce::Rectangle<int> bounds);
    void drawSelection(juce::Graphics& g, juce::Rectangle<int> bounds);
    void drawTimeMarkers(juce::Graphics& g, juce::Rectangle<int> bounds);
    
    double pixelToTime(int x, juce::Rectangle<int> bounds) const;
    int timeToPixel(double time, juce::Rectangle<int> bounds) const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};

} // namespace ForensEQ
