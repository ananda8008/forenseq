#include "SuggestionDrawerComponent.h"

namespace ForensEQ {

SuggestionDrawerComponent::SuggestionDrawerComponent()
    : animator(5, 0.3, 0.7)
{
    // Set up title label
    titleLabel.setText("AI Suggestions", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    titleLabel.setColour(juce::Label::textColourId, textColor);
    titleLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(titleLabel);
    
    // Set up expand button
    expandButton.setButtonText(">");
    expandButton.addListener(this);
    expandButton.setColour(juce::TextButton::buttonColourId, headerColor);
    expandButton.setColour(juce::TextButton::textColourOffId, textColor);
    addAndMakeVisible(expandButton);
    
    // Set up stem filter combo box
    stemFilterComboBox.addItem("All Stems", 1);
    stemFilterComboBox.addItem("Full Mix", 2);
    stemFilterComboBox.addItem("Kick", 3);
    stemFilterComboBox.addItem("Snare", 4);
    stemFilterComboBox.addItem("Bass", 5);
    stemFilterComboBox.addItem("Vocals", 6);
    stemFilterComboBox.addItem("Other", 7);
    stemFilterComboBox.setSelectedId(1);
    stemFilterComboBox.setColour(juce::ComboBox::backgroundColourId, headerColor);
    stemFilterComboBox.setColour(juce::ComboBox::textColourId, textColor);
    stemFilterComboBox.onChange = [this] { stemFilterChanged(); };
    addAndMakeVisible(stemFilterComboBox);
    
    // Set up type filter combo box
    typeFilterComboBox.addItem("All Types", 1);
    typeFilterComboBox.addItem("EQ Balance", 2);
    typeFilterComboBox.addItem("Loudness", 3);
    typeFilterComboBox.addItem("Stereo Width", 4);
    typeFilterComboBox.addItem("Dynamics", 5);
    typeFilterComboBox.setSelectedId(1);
    typeFilterComboBox.setColour(juce::ComboBox::backgroundColourId, headerColor);
    typeFilterComboBox.setColour(juce::ComboBox::textColourId, textColor);
    typeFilterComboBox.onChange = [this] { typeFilterChanged(); };
    addAndMakeVisible(typeFilterComboBox);
    
    // Set up suggestion list
    suggestionList.onSuggestionSelected = [this](const MixSuggestion& suggestion) {
        if (onSuggestionSelected)
            onSuggestionSelected(suggestion);
    };
    
    suggestionList.onSuggestionDoubleClicked = [this](const MixSuggestion& suggestion) {
        if (onSuggestionApplied)
            onSuggestionApplied(suggestion);
    };
    
    addAndMakeVisible(suggestionList);
    
    // Initialize animation
    currentWidth = collapsedWidth;
    targetWidth = collapsedWidth;
    animator.setValue(0.0f);
}

SuggestionDrawerComponent::~SuggestionDrawerComponent()
{
}

void SuggestionDrawerComponent::paint(juce::Graphics& g)
{
    // Fill background
    g.fillAll(backgroundColor);
    
    // Draw header background
    juce::Rectangle<int> headerBounds(0, 0, getWidth(), 60);
    g.setColour(headerColor);
    g.fillRect(headerBounds);
    
    // Draw border
    g.setColour(juce::Colour(60, 60, 60));
    g.drawRect(getLocalBounds(), 1);
}

void SuggestionDrawerComponent::resized()
{
    // Calculate current width based on animation
    currentWidth = collapsedWidth + (expandedWidth - collapsedWidth) * animator.getValue();
    
    // Set component bounds
    int headerHeight = 60;
    int buttonSize = 30;
    int comboBoxHeight = 25;
    int padding = 5;
    
    // Expand button is always visible
    expandButton.setBounds(padding, padding, buttonSize, buttonSize);
    
    // Title label is only visible when expanded
    if (expanded)
    {
        titleLabel.setBounds(buttonSize + padding * 2, padding, 
                            currentWidth - buttonSize - padding * 3, buttonSize);
        
        // Filter combo boxes
        stemFilterComboBox.setBounds(padding, buttonSize + padding * 2, 
                                    (currentWidth - padding * 3) / 2, comboBoxHeight);
        
        typeFilterComboBox.setBounds(stemFilterComboBox.getRight() + padding, buttonSize + padding * 2, 
                                    (currentWidth - padding * 3) / 2, comboBoxHeight);
        
        // Suggestion list
        suggestionList.setBounds(padding, headerHeight + padding, 
                                currentWidth - padding * 2, getHeight() - headerHeight - padding * 2);
    }
    else
    {
        titleLabel.setBounds(0, 0, 0, 0);
        stemFilterComboBox.setBounds(0, 0, 0, 0);
        typeFilterComboBox.setBounds(0, 0, 0, 0);
        suggestionList.setBounds(0, 0, 0, 0);
    }
}

void SuggestionDrawerComponent::setSuggestionManager(SuggestionManager* manager)
{
    suggestionList.setSuggestionManager(manager);
}

void SuggestionDrawerComponent::setExpanded(bool shouldBeExpanded)
{
    if (expanded != shouldBeExpanded)
    {
        expanded = shouldBeExpanded;
        
        // Update button text
        expandButton.setButtonText(expanded ? "<" : ">");
        
        // Update target width for animation
        targetWidth = expanded ? expandedWidth : collapsedWidth;
        
        // Start animation
        animator.setValue(expanded ? 1.0f : 0.0f);
        
        // Trigger callback
        if (onExpandedStateChanged)
            onExpandedStateChanged(expanded);
        
        // Update layout
        resized();
    }
}

void SuggestionDrawerComponent::buttonClicked(juce::Button* button)
{
    if (button == &expandButton)
    {
        setExpanded(!expanded);
    }
}

void SuggestionDrawerComponent::updateAnimation()
{
    // Update width based on animation
    currentWidth = collapsedWidth + (expandedWidth - collapsedWidth) * animator.getValue();
    
    // Update layout
    resized();
    repaint();
}

void SuggestionDrawerComponent::stemFilterChanged()
{
    int selectedId = stemFilterComboBox.getSelectedId();
    
    if (selectedId == 1)
    {
        // All stems
        suggestionList.clearFilters();
    }
    else
    {
        // Specific stem
        juce::String stemName;
        
        switch (selectedId)
        {
            case 2: stemName = "Full Mix"; break;
            case 3: stemName = "Kick"; break;
            case 4: stemName = "Snare"; break;
            case 5: stemName = "Bass"; break;
            case 6: stemName = "Vocals"; break;
            case 7: stemName = "Other"; break;
            default: stemName = "";
        }
        
        suggestionList.filterByStem(stemName);
    }
}

void SuggestionDrawerComponent::typeFilterChanged()
{
    int selectedId = typeFilterComboBox.getSelectedId();
    
    if (selectedId == 1)
    {
        // All types
        suggestionList.clearFilters();
    }
    else
    {
        // Specific type
        SuggestionType type;
        
        switch (selectedId)
        {
            case 2: type = SuggestionType::EQBalance; break;
            case 3: type = SuggestionType::Loudness; break;
            case 4: type = SuggestionType::StereoWidth; break;
            case 5: type = SuggestionType::Dynamics; break;
            default: type = SuggestionType::General;
        }
        
        suggestionList.filterByType(type);
    }
}

} // namespace ForensEQ
