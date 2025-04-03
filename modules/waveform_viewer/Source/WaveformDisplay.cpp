#include "WaveformDisplay.h"

namespace ForensEQ {

WaveformDisplay::WaveformDisplay()
{
    // Set component properties
    setOpaque(true);
}

WaveformDisplay::~WaveformDisplay()
{
    // Remove change listener if we have a thumbnail
    if (audioThumbnail != nullptr)
        audioThumbnail->removeChangeListener(this);
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    // Fill background
    g.fillAll(backgroundColor);
    
    // Get bounds for drawing
    auto bounds = getLocalBounds().reduced(2);
    
    // Draw the waveform if we have a thumbnail
    if (audioThumbnail != nullptr && audioThumbnail->getTotalLength() > 0.0)
    {
        // Draw the waveform
        drawWaveform(g, bounds);
        
        // Draw the selection if active
        if (selectionEnd > selectionStart)
        {
            drawSelection(g, bounds);
        }
        
        // Draw the playback position
        drawPlaybackPosition(g, bounds);
        
        // Draw time markers if enabled
        if (showTimeMarkers)
        {
            drawTimeMarkers(g, bounds);
        }
    }
    else
    {
        // Draw placeholder text if no waveform is available
        g.setColour(textColor);
        g.setFont(16.0f);
        g.drawText("No waveform available", bounds, juce::Justification::centred);
    }
}

void WaveformDisplay::resized()
{
    // Nothing specific needed here
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    // Repaint when the thumbnail changes
    if (source == audioThumbnail)
        repaint();
}

void WaveformDisplay::setAudioThumbnail(juce::AudioThumbnail* thumbnail)
{
    // Remove listener from old thumbnail
    if (audioThumbnail != nullptr)
        audioThumbnail->removeChangeListener(this);
    
    // Set new thumbnail
    audioThumbnail = thumbnail;
    
    // Add listener to new thumbnail
    if (audioThumbnail != nullptr)
        audioThumbnail->addChangeListener(this);
    
    // Update display
    repaint();
}

void WaveformDisplay::setTimeRange(double startTimeSeconds, double endTimeSeconds)
{
    startTime = startTimeSeconds;
    endTime = endTimeSeconds;
    repaint();
}

void WaveformDisplay::setPlaybackPosition(double positionSeconds)
{
    playbackPosition = positionSeconds;
    repaint();
}

void WaveformDisplay::setSelectionRange(double startTimeSeconds, double endTimeSeconds)
{
    selectionStart = startTimeSeconds;
    selectionEnd = endTimeSeconds;
    repaint();
}

void WaveformDisplay::setWaveformColor(juce::Colour color)
{
    waveformColor = color;
    repaint();
}

void WaveformDisplay::setBackgroundColor(juce::Colour color)
{
    backgroundColor = color;
    repaint();
}

void WaveformDisplay::setPlaybackPositionColor(juce::Colour color)
{
    playPositionColor = color;
    repaint();
}

void WaveformDisplay::setSelectionColor(juce::Colour color)
{
    selectionColor = color;
    repaint();
}

void WaveformDisplay::setShowTimeMarkers(bool show)
{
    showTimeMarkers = show;
    repaint();
}

void WaveformDisplay::setShowStereoChannels(bool show)
{
    showStereoChannels = show;
    repaint();
}

void WaveformDisplay::setVerticalZoom(float zoomFactor)
{
    verticalZoom = zoomFactor;
    repaint();
}

void WaveformDisplay::drawWaveform(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    if (audioThumbnail == nullptr)
        return;
    
    g.setColour(waveformColor);
    
    if (showStereoChannels && audioThumbnail->getNumChannels() > 1)
    {
        // Draw each channel separately
        auto channelHeight = bounds.getHeight() / 2;
        
        // Draw left channel
        auto leftBounds = bounds.removeFromTop(channelHeight);
        audioThumbnail->drawChannel(g, leftBounds,
                                  startTime, endTime,
                                  0, // channel 0 (left)
                                  verticalZoom);
        
        // Draw right channel
        auto rightBounds = bounds;
        audioThumbnail->drawChannel(g, rightBounds,
                                   startTime, endTime,
                                   1, // channel 1 (right)
                                   verticalZoom);
    }
    else
    {
        // Draw all channels together
        audioThumbnail->drawChannels(g, bounds,
                                   startTime, endTime,
                                   verticalZoom);
    }
}

void WaveformDisplay::drawPlaybackPosition(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    // Only draw if the position is within the visible range
    if (playbackPosition >= startTime && playbackPosition <= endTime)
    {
        g.setColour(playPositionColor);
        
        // Convert position to pixel position
        int pixelPosition = timeToPixel(playbackPosition, bounds);
        
        // Draw a vertical line at the position
        g.drawVerticalLine(pixelPosition, bounds.getY(), bounds.getBottom());
        
        // Draw a small triangle at the top
        juce::Path triangle;
        int triangleSize = 8;
        triangle.addTriangle(pixelPosition - triangleSize/2, bounds.getY(),
                            pixelPosition + triangleSize/2, bounds.getY(),
                            pixelPosition, bounds.getY() + triangleSize);
        g.fillPath(triangle);
    }
}

void WaveformDisplay::drawSelection(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    // Only draw if the selection is within the visible range
    if (selectionEnd > selectionStart &&
        ((selectionStart >= startTime && selectionStart <= endTime) ||
         (selectionEnd >= startTime && selectionEnd <= endTime) ||
         (selectionStart <= startTime && selectionEnd >= endTime)))
    {
        // Convert selection times to pixel positions
        int startPixel = timeToPixel(juce::jmax(selectionStart, startTime), bounds);
        int endPixel = timeToPixel(juce::jmin(selectionEnd, endTime), bounds);
        
        // Draw the selection rectangle
        g.setColour(selectionColor.withAlpha(0.3f));
        g.fillRect(startPixel, bounds.getY(), endPixel - startPixel, bounds.getHeight());
        
        // Draw the selection borders
        g.setColour(selectionColor);
        g.drawVerticalLine(startPixel, bounds.getY(), bounds.getBottom());
        g.drawVerticalLine(endPixel, bounds.getY(), bounds.getBottom());
    }
}

void WaveformDisplay::drawTimeMarkers(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    g.setColour(textColor);
    g.setFont(12.0f);
    
    // Calculate the duration of the visible range
    double visibleDuration = endTime - startTime;
    
    // Determine appropriate marker interval based on visible duration
    double markerInterval;
    if (visibleDuration <= 5.0)
        markerInterval = 0.5; // 0.5 second intervals for short durations
    else if (visibleDuration <= 20.0)
        markerInterval = 1.0; // 1 second intervals for medium durations
    else if (visibleDuration <= 60.0)
        markerInterval = 5.0; // 5 second intervals for longer durations
    else if (visibleDuration <= 300.0)
        markerInterval = 15.0; // 15 second intervals
    else
        markerInterval = 60.0; // 1 minute intervals for very long durations
    
    // Calculate the first marker time (round up to the next interval)
    double firstMarkerTime = std::ceil(startTime / markerInterval) * markerInterval;
    
    // Draw markers at regular intervals
    for (double time = firstMarkerTime; time <= endTime; time += markerInterval)
    {
        int x = timeToPixel(time, bounds);
        
        // Format time as mm:ss or mm:ss.ms depending on interval
        juce::String timeString;
        if (markerInterval >= 1.0)
        {
            // Format as mm:ss
            int minutes = static_cast<int>(time) / 60;
            int seconds = static_cast<int>(time) % 60;
            timeString = juce::String::formatted("%d:%02d", minutes, seconds);
        }
        else
        {
            // Format as mm:ss.ms
            int minutes = static_cast<int>(time) / 60;
            int seconds = static_cast<int>(time) % 60;
            int ms = static_cast<int>((time - std::floor(time)) * 100);
            timeString = juce::String::formatted("%d:%02d.%02d", minutes, seconds, ms);
        }
        
        // Draw marker line
        g.setColour(textColor.withAlpha(0.5f));
        g.drawVerticalLine(x, bounds.getY(), bounds.getBottom());
        
        // Draw time text
        g.setColour(textColor);
        g.drawText(timeString, x - 25, bounds.getBottom() - 15, 50, 15, juce::Justification::centred);
    }
}

double WaveformDisplay::pixelToTime(int x, juce::Rectangle<int> bounds) const
{
    double proportion = (x - bounds.getX()) / static_cast<double>(bounds.getWidth());
    return startTime + proportion * (endTime - startTime);
}

int WaveformDisplay::timeToPixel(double time, juce::Rectangle<int> bounds) const
{
    double proportion = (time - startTime) / (endTime - startTime);
    return bounds.getX() + static_cast<int>(proportion * bounds.getWidth());
}

} // namespace ForensEQ
