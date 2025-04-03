#pragma once

#include <JuceHeader.h>
#include "MixSuggestion.h"
#include "SuggestionManager.h"

namespace ForensEQ {

/**
 * Component that displays a single mix suggestion
 */
class SuggestionItemComponent : public juce::Component
{
public:
    SuggestionItemComponent(const MixSuggestion& suggestion);
    ~SuggestionItemComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Update the suggestion this component displays
    void updateSuggestion(const MixSuggestion& suggestion);
    
    // Get the current suggestion
    const MixSuggestion& getSuggestion() const { return suggestion; }
    
    // Set whether this suggestion is selected
    void setSelected(bool selected);
    
    // Check if this suggestion is selected
    bool isSelected() const { return selected; }

private:
    MixSuggestion suggestion;
    bool selected = false;
    
    // UI colors
    juce::Colour backgroundColor = juce::Colour(40, 40, 40);
    juce::Colour selectedBackgroundColor = juce::Colour(60, 60, 60);
    juce::Colour textColor = juce::Colour(220, 220, 220);
    juce::Colour stemNameColor = juce::Colour(180, 180, 180);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SuggestionItemComponent)
};

} // namespace ForensEQ
