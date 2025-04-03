#include "WaveformViewerComponent.h"
#include "WaveformDisplay.h"

namespace ForensEQ {

WaveformViewerComponent::WaveformViewerComponent()
    : thumbnailCache(5)  // Cache size in number of thumbnails
{
    // Register basic audio formats
    formatManager.registerBasicFormats();
    
    // Initialize reference tracks
    for (auto& track : referenceTracks)
    {
        track = ReferenceTrack(thumbnailCache, formatManager);
    }
    
    // Create and set up the waveform display component
    waveformDisplay = std::make_unique<WaveformDisplay>();
    addAndMakeVisible(waveformDisplay.get());
    
    // Set component properties
    setOpaque(true);
    
    // Start timer for animation updates
    startTimerHz(30); // 30 fps for smooth animations
}

WaveformViewerComponent::~WaveformViewerComponent()
{
    stopTimer();
}

void WaveformViewerComponent::paint(juce::Graphics& g)
{
    // Fill background
    g.fillAll(backgroundColor);
    
    // The actual waveform drawing is now handled by the WaveformDisplay component
    // This method just draws any additional UI elements or instructions
    
    // Get the current reference track
    const auto& currentTrack = referenceTracks[currentTrackIndex];
    
    // If no track is loaded, show instructions
    if (!currentTrack.loaded)
    {
        // Draw placeholder text if no track is loaded
        g.setColour(textColor);
        g.setFont(16.0f);
        g.drawText("Drag and drop audio files here or click to load a reference track",
                  getLocalBounds(), juce::Justification::centred);
    }
}

void WaveformViewerComponent::resized()
{
    // Position the waveform display to fill the entire component
    if (waveformDisplay != nullptr)
    {
        waveformDisplay->setBounds(getLocalBounds());
    }
}

void WaveformViewerComponent::timerCallback()
{
    // Update animation or playback position if needed
    // For now, just trigger a repaint to ensure UI is responsive
    repaint();
}

bool WaveformViewerComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    // Check if any of the files are supported audio formats
    for (const auto& file : files)
    {
        if (formatManager.findFormatForFileExtension(juce::File(file).getFileExtension()))
            return true;
    }
    
    return false;
}

void WaveformViewerComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    // Find the first valid audio file
    for (const auto& file : files)
    {
        juce::File audioFile(file);
        if (formatManager.findFormatForFileExtension(audioFile.getFileExtension()))
        {
            // Load the file as the current reference track
            loadReferenceTrack(audioFile, currentTrackIndex);
            break;
        }
    }
}

bool WaveformViewerComponent::loadReferenceTrack(const juce::File& file, int trackIndex)
{
    // Validate track index
    if (trackIndex < 0 || trackIndex >= maxReferenceTracks)
        return false;
    
    // Get a reader for this file
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    
    if (reader != nullptr)
    {
        // Update the reference track
        auto& track = referenceTracks[trackIndex];
        track.file = file;
        track.reader = std::move(reader);
        track.name = file.getFileNameWithoutExtension();
        track.lengthInSeconds = track.reader->lengthInSamples / track.reader->sampleRate;
        
        // Clear the thumbnail and set its new source
        track.thumbnail.clear();
        track.thumbnail.setSource(new juce::FileInputSource(file));
        
        // Mark as loaded
        track.loaded = true;
        
        // Switch to this track
        currentTrackIndex = trackIndex;
        
        // Reset selection and position
        currentPosition = 0.0;
        selectionStart = 0.0;
        selectionEnd = 0.0;
        
        // Update the waveform display
        if (waveformDisplay != nullptr)
        {
            waveformDisplay->setAudioThumbnail(&track.thumbnail);
            waveformDisplay->setTimeRange(0.0, track.lengthInSeconds);
            waveformDisplay->setPlaybackPosition(currentPosition);
            waveformDisplay->setSelectionRange(selectionStart, selectionEnd);
        }
        
        // Trigger a repaint
        repaint();
        
        return true;
    }
    
    return false;
}

int WaveformViewerComponent::getNumReferenceTracks() const
{
    int count = 0;
    for (const auto& track : referenceTracks)
    {
        if (track.loaded)
            count++;
    }
    
    return count;
}

void WaveformViewerComponent::switchToReferenceTrack(int trackIndex)
{
    // Validate track index and check if it's loaded
    if (trackIndex >= 0 && trackIndex < maxReferenceTracks && referenceTracks[trackIndex].loaded)
    {
        currentTrackIndex = trackIndex;
        repaint();
    }
}

int WaveformViewerComponent::getCurrentReferenceTrackIndex() const
{
    return currentTrackIndex;
}

void WaveformViewerComponent::mouseDown(const juce::MouseEvent& e)
{
    const auto& currentTrack = referenceTracks[currentTrackIndex];
    
    if (currentTrack.loaded)
    {
        auto bounds = getLocalBounds().reduced(2);
        
        // Set the current position based on click position
        currentPosition = pixelToTime(e.x, bounds);
        
        // Start selection
        selectionStart = currentPosition;
        selectionEnd = currentPosition;
        isSelecting = true;
        
        // Update waveform display
        if (waveformDisplay != nullptr)
        {
            waveformDisplay->setPlaybackPosition(currentPosition);
            waveformDisplay->setSelectionRange(selectionStart, selectionEnd);
        }
        
        repaint();
    }
    else
    {
        // If no track is loaded, show file chooser
        juce::FileChooser chooser("Select a reference audio file",
                                 juce::File::getSpecialLocation(juce::File::userHomeDirectory),
                                 formatManager.getWildcardForAllFormats());
        
        if (chooser.browseForFileToOpen())
        {
            loadReferenceTrack(chooser.getResult(), currentTrackIndex);
        }
    }
}

void WaveformViewerComponent::mouseDrag(const juce::MouseEvent& e)
{
    const auto& currentTrack = referenceTracks[currentTrackIndex];
    
    if (currentTrack.loaded && isSelecting)
    {
        auto bounds = getLocalBounds().reduced(2);
        
        // Update the selection end based on drag position
        selectionEnd = pixelToTime(e.x, bounds);
        
        // Ensure selection is in the correct order
        if (selectionEnd < selectionStart)
        {
            std::swap(selectionStart, selectionEnd);
        }
        
        // Update waveform display
        if (waveformDisplay != nullptr)
        {
            waveformDisplay->setSelectionRange(selectionStart, selectionEnd);
        }
        
        repaint();
    }
}

void WaveformViewerComponent::mouseUp(const juce::MouseEvent& e)
{
    isSelecting = false;
    
    // If the selection is very small, treat it as a click (seek only)
    if (std::abs(selectionEnd - selectionStart) < 0.05)
    {
        selectionStart = 0.0;
        selectionEnd = 0.0;
        
        // Update waveform display to clear selection
        if (waveformDisplay != nullptr)
        {
            waveformDisplay->setSelectionRange(selectionStart, selectionEnd);
        }
    }
    
    repaint();
}

void WaveformViewerComponent::drawWaveform(juce::Graphics& g, const ReferenceTrack& track, juce::Rectangle<int> bounds)
{
    g.setColour(waveformColor);
    
    // Draw the thumbnail
    track.thumbnail.drawChannels(g, bounds,
                               0.0, // start time
                               track.lengthInSeconds, // end time
                               1.0f); // vertical zoom
}

void WaveformViewerComponent::drawPlaybackPosition(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    g.setColour(playPositionColor);
    
    // Convert current position to pixel position
    int pixelPosition = timeToPixel(currentPosition, bounds);
    
    // Draw a vertical line at the current position
    g.drawVerticalLine(pixelPosition, bounds.getY(), bounds.getBottom());
    
    // Draw a small triangle at the top
    juce::Path triangle;
    int triangleSize = 8;
    triangle.addTriangle(pixelPosition - triangleSize/2, bounds.getY(),
                        pixelPosition + triangleSize/2, bounds.getY(),
                        pixelPosition, bounds.getY() + triangleSize);
    g.fillPath(triangle);
}

void WaveformViewerComponent::drawSelection(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    // Convert selection times to pixel positions
    int startPixel = timeToPixel(selectionStart, bounds);
    int endPixel = timeToPixel(selectionEnd, bounds);
    
    // Draw the selection rectangle
    g.setColour(selectionColor.withAlpha(0.3f));
    g.fillRect(startPixel, bounds.getY(), endPixel - startPixel, bounds.getHeight());
    
    // Draw the selection borders
    g.setColour(selectionColor);
    g.drawVerticalLine(startPixel, bounds.getY(), bounds.getBottom());
    g.drawVerticalLine(endPixel, bounds.getY(), bounds.getBottom());
}

void WaveformViewerComponent::drawTimeMarkers(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    const auto& currentTrack = referenceTracks[currentTrackIndex];
    
    g.setColour(textColor);
    g.setFont(12.0f);
    
    // Draw time markers at regular intervals
    int numMarkers = 5; // Number of markers to display
    for (int i = 0; i <= numMarkers; ++i)
    {
        double time = (i * currentTrack.lengthInSeconds) / numMarkers;
        int x = timeToPixel(time, bounds);
        
        // Format time as mm:ss
        int minutes = static_cast<int>(time) / 60;
        int seconds = static_cast<int>(time) % 60;
        juce::String timeString = juce::String::formatted("%d:%02d", minutes, seconds);
        
        // Draw marker line
        g.setColour(textColor.withAlpha(0.5f));
        g.drawVerticalLine(x, bounds.getY(), bounds.getBottom());
        
        // Draw time text
        g.setColour(textColor);
        g.drawText(timeString, x - 20, bounds.getBottom() - 15, 40, 15, juce::Justification::centred);
    }
}

double WaveformViewerComponent::pixelToTime(int x, juce::Rectangle<int> bounds) const
{
    const auto& currentTrack = referenceTracks[currentTrackIndex];
    
    if (currentTrack.loaded)
    {
        double proportion = (x - bounds.getX()) / static_cast<double>(bounds.getWidth());
        return juce::jlimit(0.0, currentTrack.lengthInSeconds, proportion * currentTrack.lengthInSeconds);
    }
    
    return 0.0;
}

int WaveformViewerComponent::timeToPixel(double time, juce::Rectangle<int> bounds) const
{
    const auto& currentTrack = referenceTracks[currentTrackIndex];
    
    if (currentTrack.loaded && currentTrack.lengthInSeconds > 0.0)
    {
        double proportion = time / currentTrack.lengthInSeconds;
        return bounds.getX() + static_cast<int>(proportion * bounds.getWidth());
    }
    
    return bounds.getX();
}

} // namespace ForensEQ
