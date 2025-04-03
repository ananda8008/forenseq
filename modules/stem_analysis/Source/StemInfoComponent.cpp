#include "StemInfoComponent.h"

namespace ForensEQ {

StemInfoComponent::StemInfoComponent(StemManager& stemManager)
    : stemManager(stemManager)
{
    // Set up title label
    titleLabel.setText("Stem Information", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, textColor);
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    
    // Set up stem type label
    stemTypeLabel.setColour(juce::Label::textColourId, textColor);
    stemTypeLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(stemTypeLabel);
    
    // Set up LUFS label
    lufsLabel.setColour(juce::Label::textColourId, textColor);
    lufsLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(lufsLabel);
    
    // Set up RMS label
    rmsLabel.setColour(juce::Label::textColourId, textColor);
    rmsLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(rmsLabel);
    
    // Set up width label
    widthLabel.setColour(juce::Label::textColourId, textColor);
    widthLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(widthLabel);
    
    // Set up comparison label
    comparisonLabel.setColour(juce::Label::textColourId, textColor);
    comparisonLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(comparisonLabel);
    
    // Listen for changes in the stem manager
    stemManager.addChangeListener(this);
    
    // Update the UI to reflect the current state
    updateUI();
}

StemInfoComponent::~StemInfoComponent()
{
    stemManager.removeChangeListener(this);
}

void StemInfoComponent::paint(juce::Graphics& g)
{
    g.fillAll(backgroundColor);
    
    // Draw a border
    g.setColour(textColor.withAlpha(0.3f));
    g.drawRect(getLocalBounds(), 1);
}

void StemInfoComponent::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
    // Title at the top
    titleLabel.setBounds(bounds.removeFromTop(25));
    
    bounds.removeFromTop(10); // Spacing
    
    // Information labels
    int labelHeight = 20;
    int spacing = 5;
    
    stemTypeLabel.setBounds(bounds.removeFromTop(labelHeight));
    bounds.removeFromTop(spacing);
    
    lufsLabel.setBounds(bounds.removeFromTop(labelHeight));
    bounds.removeFromTop(spacing);
    
    rmsLabel.setBounds(bounds.removeFromTop(labelHeight));
    bounds.removeFromTop(spacing);
    
    widthLabel.setBounds(bounds.removeFromTop(labelHeight));
    bounds.removeFromTop(spacing);
    
    comparisonLabel.setBounds(bounds.removeFromTop(labelHeight));
}

void StemInfoComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &stemManager)
    {
        // Update the UI when the stem manager changes
        updateUI();
    }
}

void StemInfoComponent::updateUI()
{
    // Get the current active stem
    StemType activeStemType = stemManager.getActiveStemType();
    StemData* activeStem = stemManager.getStem(activeStemType);
    
    if (activeStem != nullptr)
    {
        // Update stem type
        stemTypeLabel.setText("Type: " + activeStem->getName(), juce::dontSendNotification);
        
        // Update LUFS
        float lufs = activeStem->getLUFS();
        lufsLabel.setText("LUFS: " + juce::String(lufs, 1) + " dB", juce::dontSendNotification);
        
        // Update RMS
        float rms = activeStem->getRMS();
        rmsLabel.setText("RMS: " + juce::String(rms, 3), juce::dontSendNotification);
        
        // Update width
        float width = activeStem->getWidth();
        widthLabel.setText("Width: " + juce::String(width * 100.0f, 1) + "%", juce::dontSendNotification);
        
        // Update comparison with reference (full mix)
        if (activeStemType != StemType::Full && stemManager.isStemAvailable(StemType::Full))
        {
            float similarity = stemManager.compareWithReference(StemType::Full);
            comparisonLabel.setText("Similarity to Mix: " + juce::String(similarity * 100.0f, 1) + "%", 
                                   juce::dontSendNotification);
        }
        else
        {
            comparisonLabel.setText("", juce::dontSendNotification);
        }
    }
    else
    {
        // Clear all labels if no stem is selected
        stemTypeLabel.setText("No stem selected", juce::dontSendNotification);
        lufsLabel.setText("", juce::dontSendNotification);
        rmsLabel.setText("", juce::dontSendNotification);
        widthLabel.setText("", juce::dontSendNotification);
        comparisonLabel.setText("", juce::dontSendNotification);
    }
}

} // namespace ForensEQ
