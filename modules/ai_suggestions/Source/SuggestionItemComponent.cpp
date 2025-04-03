#include "SuggestionItemComponent.h"

namespace ForensEQ {

SuggestionItemComponent::SuggestionItemComponent(const MixSuggestion& suggestion)
    : suggestion(suggestion)
{
    setInterceptsMouseClicks(true, true);
}

SuggestionItemComponent::~SuggestionItemComponent()
{
}

void SuggestionItemComponent::paint(juce::Graphics& g)
{
    // Fill background
    g.fillAll(selected ? selectedBackgroundColor : backgroundColor);
    
    // Draw border
    g.setColour(suggestion.getColor().withAlpha(0.7f));
    g.drawRect(getLocalBounds(), 1);
    
    // Draw suggestion type indicator
    juce::Rectangle<int> indicatorBounds(0, 0, 5, getHeight());
    g.setColour(suggestion.getColor());
    g.fillRect(indicatorBounds);
    
    // Draw stem name
    g.setFont(14.0f);
    g.setColour(stemNameColor);
    g.drawText(suggestion.getStemName() + ":", 10, 5, getWidth() - 15, 20, juce::Justification::topLeft);
    
    // Draw suggestion description
    g.setFont(16.0f);
    g.setColour(textColor);
    g.drawText(suggestion.getDescription(), 10, 25, getWidth() - 15, getHeight() - 30, juce::Justification::topLeft);
    
    // Draw suggestion type
    g.setFont(12.0f);
    g.setColour(suggestion.getColor());
    g.drawText(MixSuggestion::suggestionTypeToString(suggestion.getType()), 
               getWidth() - 100, 5, 90, 20, juce::Justification::topRight);
    
    // If suggestion is applied, draw a checkmark or indicator
    if (suggestion.isApplied())
    {
        g.setColour(juce::Colours::green.withAlpha(0.7f));
        g.drawText("✓", getWidth() - 20, 5, 15, 15, juce::Justification::centred);
    }
}

void SuggestionItemComponent::resized()
{
    // Nothing specific needed here
}

void SuggestionItemComponent::updateSuggestion(const MixSuggestion& newSuggestion)
{
    suggestion = newSuggestion;
    repaint();
}

void SuggestionItemComponent::setSelected(bool isSelected)
{
    selected = isSelected;
    repaint();
}

} // namespace ForensEQ
