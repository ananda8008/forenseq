#include "StemAnalysisComponent.h"

namespace ForensEQ {

StemAnalysisComponent::StemAnalysisComponent()
{
    // Add child components
    addAndMakeVisible(stemSelector);
    addAndMakeVisible(stemInfo);
    
    // Set size
    setSize(800, 400);
}

StemAnalysisComponent::~StemAnalysisComponent()
{
}

void StemAnalysisComponent::paint(juce::Graphics& g)
{
    g.fillAll(backgroundColor);
    
    // If no reference track is loaded, show instructions
    if (stemManager.getNumLoadedStems() == 0)
    {
        g.setColour(textColor);
        g.setFont(16.0f);
        g.drawText("Drag and drop an audio file to analyze stems",
                  getLocalBounds(), juce::Justification::centred);
    }
}

void StemAnalysisComponent::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
    // Position the stem selector on the left
    int selectorWidth = 150;
    stemSelector.setBounds(bounds.removeFromLeft(selectorWidth));
    
    bounds.removeFromLeft(10); // Spacing
    
    // Position the stem info on the right
    int infoWidth = 200;
    stemInfo.setBounds(bounds.removeFromRight(infoWidth));
    
    // The remaining space is for future components or visualization
}

bool StemAnalysisComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    // Check if any of the files are supported audio formats
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    for (const auto& file : files)
    {
        if (formatManager.findFormatForFileExtension(juce::File(file).getFileExtension()))
            return true;
    }
    
    return false;
}

void StemAnalysisComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    // Find the first valid audio file
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    for (const auto& file : files)
    {
        juce::File audioFile(file);
        if (formatManager.findFormatForFileExtension(audioFile.getFileExtension()))
        {
            loadReferenceTrack(audioFile);
            break;
        }
    }
}

bool StemAnalysisComponent::loadReferenceTrack(const juce::File& file)
{
    return stemManager.loadReferenceTrack(file);
}

StemType StemAnalysisComponent::getSelectedStemType() const
{
    return stemManager.getActiveStemType();
}

void StemAnalysisComponent::getSelectedStemFrequencyData(std::vector<float>& frequencies, std::vector<float>& magnitudes) const
{
    StemType activeStemType = stemManager.getActiveStemType();
    StemData* activeStem = stemManager.getStem(activeStemType);
    
    if (activeStem != nullptr && activeStem->hasValidFrequencyData())
    {
        activeStem->getFrequencyData(frequencies, magnitudes);
    }
    else
    {
        frequencies.clear();
        magnitudes.clear();
    }
}

} // namespace ForensEQ
