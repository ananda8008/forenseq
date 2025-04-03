#include "WaveformViewerComponent.h"
#include "WaveformDisplay.h"

namespace ForensEQ {

// Update the WaveformViewerComponent to use the new WaveformDisplay
void WaveformViewerComponent::resized()
{
    // Update layout to use the WaveformDisplay component
    // This would be implemented in a full integration
}

// Add a method to get the waveform display component
WaveformDisplay* WaveformViewerComponent::getWaveformDisplay()
{
    // This would return a pointer to the WaveformDisplay component
    // For now, it's just a placeholder
    return nullptr;
}

// Add a method to get track information
juce::String WaveformViewerComponent::getTrackName(int trackIndex) const
{
    if (trackIndex >= 0 && trackIndex < maxReferenceTracks && referenceTracks[trackIndex].loaded)
    {
        return referenceTracks[trackIndex].name;
    }
    return "No Track";
}

// Add a method to get track duration
double WaveformViewerComponent::getTrackDuration(int trackIndex) const
{
    if (trackIndex >= 0 && trackIndex < maxReferenceTracks && referenceTracks[trackIndex].loaded)
    {
        return referenceTracks[trackIndex].lengthInSeconds;
    }
    return 0.0;
}

// Add a method to get track sample rate
double WaveformViewerComponent::getTrackSampleRate(int trackIndex) const
{
    if (trackIndex >= 0 && trackIndex < maxReferenceTracks && 
        referenceTracks[trackIndex].loaded && referenceTracks[trackIndex].reader != nullptr)
    {
        return referenceTracks[trackIndex].reader->sampleRate;
    }
    return 0.0;
}

// Add a method to get track channel count
int WaveformViewerComponent::getTrackNumChannels(int trackIndex) const
{
    if (trackIndex >= 0 && trackIndex < maxReferenceTracks && 
        referenceTracks[trackIndex].loaded && referenceTracks[trackIndex].reader != nullptr)
    {
        return referenceTracks[trackIndex].reader->numChannels;
    }
    return 0;
}

} // namespace ForensEQ
