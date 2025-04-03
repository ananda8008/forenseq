#pragma once

#include <JuceHeader.h>
#include "SuggestionListComponent.h"
#include "SuggestionManager.h"

namespace ForensEQ {

/**
 * Component that displays suggestions in an expandable drawer
 */
class SuggestionDrawerComponent : public juce::Component,
                                 public juce::Button::Listener
{
public:
    SuggestionDrawerComponent();
    ~SuggestionDrawerComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Set the suggestion manager to display
    void setSuggestionManager(SuggestionManager* manager);
    
    // Set whether the drawer is expanded
    void setExpanded(bool shouldBeExpanded);
    
    // Check if the drawer is expanded
    bool isExpanded() const { return expanded; }
    
    // Button::Listener override
    void buttonClicked(juce::Button* button) override;
    
    // Callback when a suggestion is selected
    std::function<void(const MixSuggestion&)> onSuggestionSelected;
    
    // Callback when a suggestion is applied
    std::function<void(const MixSuggestion&)> onSuggestionApplied;
    
    // Callback when the drawer is expanded or collapsed
    std::function<void(bool)> onExpandedStateChanged;

private:
    bool expanded = false;
    
    // UI components
    SuggestionListComponent suggestionList;
    juce::TextButton expandButton;
    juce::ComboBox stemFilterComboBox;
    juce::ComboBox typeFilterComboBox;
    juce::Label titleLabel;
    
    // UI colors
    juce::Colour backgroundColor = juce::Colour(30, 30, 30);
    juce::Colour headerColor = juce::Colour(40, 40, 40);
    juce::Colour textColor = juce::Colour(220, 220, 220);
    
    // Animation
    float currentWidth = 0.0f;
    float targetWidth = 0.0f;
    const float collapsedWidth = 30.0f;
    const float expandedWidth = 300.0f;
    juce::AnimatedPosition<float> animator;
    
    // Update the animation
    void updateAnimation();
    
    // Handle stem filter change
    void stemFilterChanged();
    
    // Handle type filter change
    void typeFilterChanged();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SuggestionDrawerComponent)
};

} // namespace ForensEQ
