#include "StemSelectorComponent.h"

namespace ForensEQ {

StemSelectorComponent::StemSelectorComponent(StemManager& stemManager)
    : stemManager(stemManager)
{
    // Add buttons for each stem type
    addAndMakeVisible(fullMixButton);
    addAndMakeVisible(kickButton);
    addAndMakeVisible(snareButton);
    addAndMakeVisible(bassButton);
    addAndMakeVisible(vocalsButton);
    addAndMakeVisible(otherButton);
    
    // Add listeners to buttons
    fullMixButton.addListener(this);
    kickButton.addListener(this);
    snareButton.addListener(this);
    bassButton.addListener(this);
    vocalsButton.addListener(this);
    otherButton.addListener(this);
    
    // Style the buttons
    for (auto* button : {&fullMixButton, &kickButton, &snareButton, &bassButton, &vocalsButton, &otherButton})
    {
        button->setColour(juce::TextButton::buttonColourId, disabledColor);
        button->setColour(juce::TextButton::textColourOffId, textColor);
    }
    
    // Set up status label
    statusLabel.setColour(juce::Label::textColourId, textColor);
    statusLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(statusLabel);
    
    // Listen for changes in the stem manager
    stemManager.addChangeListener(this);
    
    // Update the UI to reflect the current state
    updateUI();
}

StemSelectorComponent::~StemSelectorComponent()
{
    // Remove listeners
    fullMixButton.removeListener(this);
    kickButton.removeListener(this);
    snareButton.removeListener(this);
    bassButton.removeListener(this);
    vocalsButton.removeListener(this);
    otherButton.removeListener(this);
    
    stemManager.removeChangeListener(this);
}

void StemSelectorComponent::paint(juce::Graphics& g)
{
    g.fillAll(backgroundColor);
    
    g.setColour(textColor);
    g.setFont(16.0f);
    g.drawText("Stem Selection", getLocalBounds().removeFromTop(30), juce::Justification::centred);
}

void StemSelectorComponent::resized()
{
    auto bounds = getLocalBounds();
    
    // Space for title
    bounds.removeFromTop(30);
    
    // Status label at the bottom
    statusLabel.setBounds(bounds.removeFromBottom(30));
    
    // Calculate button size
    int buttonHeight = 30;
    int buttonSpacing = 5;
    int totalButtonHeight = 6 * buttonHeight + 5 * buttonSpacing;
    
    // Center the buttons vertically
    int topMargin = (bounds.getHeight() - totalButtonHeight) / 2;
    bounds.removeFromTop(topMargin);
    
    // Position each button
    fullMixButton.setBounds(bounds.removeFromTop(buttonHeight));
    bounds.removeFromTop(buttonSpacing);
    
    kickButton.setBounds(bounds.removeFromTop(buttonHeight));
    bounds.removeFromTop(buttonSpacing);
    
    snareButton.setBounds(bounds.removeFromTop(buttonHeight));
    bounds.removeFromTop(buttonSpacing);
    
    bassButton.setBounds(bounds.removeFromTop(buttonHeight));
    bounds.removeFromTop(buttonSpacing);
    
    vocalsButton.setBounds(bounds.removeFromTop(buttonHeight));
    bounds.removeFromTop(buttonSpacing);
    
    otherButton.setBounds(bounds.removeFromTop(buttonHeight));
}

void StemSelectorComponent::buttonClicked(juce::Button* button)
{
    StemType type = StemType::Full;
    
    if (button == &fullMixButton)
        type = StemType::Full;
    else if (button == &kickButton)
        type = StemType::Kick;
    else if (button == &snareButton)
        type = StemType::Snare;
    else if (button == &bassButton)
        type = StemType::Bass;
    else if (button == &vocalsButton)
        type = StemType::Vocals;
    else if (button == &otherButton)
        type = StemType::Other;
    
    // Set the active stem type in the manager
    stemManager.setActiveStemType(type);
    
    // Update the UI
    updateUI();
}

void StemSelectorComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &stemManager)
    {
        // Update the UI when the stem manager changes
        updateUI();
    }
}

void StemSelectorComponent::updateUI()
{
    // Get the current active stem type
    StemType activeStemType = stemManager.getActiveStemType();
    
    // Get available stem types
    std::vector<StemType> availableTypes = stemManager.getAvailableStemTypes();
    
    // Reset all buttons to disabled state
    for (auto* button : {&fullMixButton, &kickButton, &snareButton, &bassButton, &vocalsButton, &otherButton})
    {
        button->setColour(juce::TextButton::buttonColourId, disabledColor);
        button->setEnabled(false);
    }
    
    // Enable available stem types and highlight the active one
    for (StemType type : availableTypes)
    {
        juce::TextButton* button = nullptr;
        
        switch (type)
        {
            case StemType::Full:
                button = &fullMixButton;
                break;
            case StemType::Kick:
                button = &kickButton;
                break;
            case StemType::Snare:
                button = &snareButton;
                break;
            case StemType::Bass:
                button = &bassButton;
                break;
            case StemType::Vocals:
                button = &vocalsButton;
                break;
            case StemType::Other:
                button = &otherButton;
                break;
        }
        
        if (button != nullptr)
        {
            button->setEnabled(true);
            
            if (type == activeStemType)
            {
                button->setColour(juce::TextButton::buttonColourId, selectedColor);
            }
            else
            {
                button->setColour(juce::TextButton::buttonColourId, juce::Colour(60, 60, 60));
            }
        }
    }
    
    // Update status label
    if (stemManager.getNumLoadedStems() > 0)
    {
        StemData* activeStem = stemManager.getStem(activeStemType);
        if (activeStem != nullptr)
        {
            statusLabel.setText("Selected: " + activeStem->getName(), juce::dontSendNotification);
        }
        else
        {
            statusLabel.setText("No stem selected", juce::dontSendNotification);
        }
    }
    else
    {
        statusLabel.setText("No reference track loaded", juce::dontSendNotification);
    }
}

} // namespace ForensEQ
