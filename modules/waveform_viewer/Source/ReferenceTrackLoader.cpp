#include "ReferenceTrackLoader.h"

namespace ForensEQ {

ReferenceTrackLoader::ReferenceTrackLoader(WaveformViewerComponent& waveformViewer)
    : waveformViewer(waveformViewer)
{
    // Register basic audio formats
    formatManager.registerBasicFormats();
    
    // Set up load buttons
    addAndMakeVisible(loadButton1);
    addAndMakeVisible(loadButton2);
    addAndMakeVisible(loadButton3);
    
    loadButton1.addListener(this);
    loadButton2.addListener(this);
    loadButton3.addListener(this);
    
    // Style the buttons
    for (auto* button : {&loadButton1, &loadButton2, &loadButton3})
    {
        button->setColour(juce::TextButton::buttonColourId, juce::Colour(60, 60, 60));
        button->setColour(juce::TextButton::textColourOffId, textColor);
    }
    
    // Set up track labels
    for (auto* label : {&trackLabel1, &trackLabel2, &trackLabel3})
    {
        label->setColour(juce::Label::textColourId, textColor);
        label->setJustificationType(juce::Justification::centredLeft);
        addAndMakeVisible(label);
    }
    
    // Update track labels
    updateTrackLabels();
}

ReferenceTrackLoader::~ReferenceTrackLoader()
{
    loadButton1.removeListener(this);
    loadButton2.removeListener(this);
    loadButton3.removeListener(this);
}

void ReferenceTrackLoader::paint(juce::Graphics& g)
{
    g.fillAll(backgroundColor);
    
    g.setColour(textColor);
    g.setFont(16.0f);
    g.drawText("Reference Tracks", 10, 5, 200, 20, juce::Justification::centredLeft);
    
    // Draw separator line
    g.setColour(textColor.withAlpha(0.3f));
    g.drawLine(0, 30, getWidth(), 30, 1.0f);
}

void ReferenceTrackLoader::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    bounds.removeFromTop(25); // Space for title
    
    // Layout for each track row
    for (int i = 0; i < 3; ++i)
    {
        auto rowBounds = bounds.removeFromTop(30);
        
        // Load button
        auto* button = (i == 0) ? &loadButton1 : (i == 1) ? &loadButton2 : &loadButton3;
        button->setBounds(rowBounds.removeFromLeft(100));
        
        rowBounds.removeFromLeft(10); // Spacing
        
        // Track label
        auto* label = (i == 0) ? &trackLabel1 : (i == 1) ? &trackLabel2 : &trackLabel3;
        label->setBounds(rowBounds);
        
        bounds.removeFromTop(5); // Spacing between rows
    }
}

bool ReferenceTrackLoader::isInterestedInFileDrag(const juce::StringArray& files)
{
    // Check if any of the files are supported audio formats
    for (const auto& file : files)
    {
        if (formatManager.findFormatForFileExtension(juce::File(file).getFileExtension()))
            return true;
    }
    
    return false;
}

void ReferenceTrackLoader::filesDropped(const juce::StringArray& files, int x, int y)
{
    // Find the first valid audio file
    for (const auto& file : files)
    {
        juce::File audioFile(file);
        if (formatManager.findFormatForFileExtension(audioFile.getFileExtension()))
        {
            // Determine which track to load based on y position
            int trackIndex = 0;
            
            // Simple logic to determine track index based on vertical position
            if (y > getHeight() / 3 && y < 2 * getHeight() / 3)
                trackIndex = 1;
            else if (y >= 2 * getHeight() / 3)
                trackIndex = 2;
            
            // Load the file
            loadReferenceTrack(audioFile, trackIndex);
            break;
        }
    }
}

void ReferenceTrackLoader::buttonClicked(juce::Button* button)
{
    int trackIndex = 0;
    
    if (button == &loadButton1)
        trackIndex = 0;
    else if (button == &loadButton2)
        trackIndex = 1;
    else if (button == &loadButton3)
        trackIndex = 2;
    
    openFileChooser(trackIndex);
}

bool ReferenceTrackLoader::loadReferenceTrack(const juce::File& file, int trackIndex)
{
    // Load the track in the waveform viewer
    bool success = waveformViewer.loadReferenceTrack(file, trackIndex);
    
    // Update track labels
    updateTrackLabels();
    
    return success;
}

void ReferenceTrackLoader::openFileChooser(int trackIndex)
{
    // Create file chooser
    juce::FileChooser chooser("Select a reference audio file",
                             juce::File::getSpecialLocation(juce::File::userHomeDirectory),
                             formatManager.getWildcardForAllFormats());
    
    // Show file chooser
    if (chooser.browseForFileToOpen())
    {
        loadReferenceTrack(chooser.getResult(), trackIndex);
    }
}

void ReferenceTrackLoader::updateTrackLabels()
{
    // Get the number of loaded tracks
    int numTracks = waveformViewer.getNumReferenceTracks();
    
    // Update track labels based on loaded tracks
    for (int i = 0; i < 3; ++i)
    {
        auto* label = (i == 0) ? &trackLabel1 : (i == 1) ? &trackLabel2 : &trackLabel3;
        
        // Check if this track is loaded
        if (i < numTracks)
        {
            // Switch to this track temporarily to get its info
            int currentTrack = waveformViewer.getCurrentReferenceTrackIndex();
            waveformViewer.switchToReferenceTrack(i);
            
            // Get track info (this would need to be implemented in WaveformViewerComponent)
            // For now, just show a placeholder
            label->setText("Track " + juce::String(i + 1) + " loaded", juce::dontSendNotification);
            
            // Switch back to the original track
            waveformViewer.switchToReferenceTrack(currentTrack);
        }
        else
        {
            label->setText("No track loaded", juce::dontSendNotification);
        }
    }
}

} // namespace ForensEQ
