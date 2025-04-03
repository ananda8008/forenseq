#pragma once

#include <JuceHeader.h>
#include "SuggestionItemComponent.h"
#include "SuggestionManager.h"

namespace ForensEQ {

/**
 * Component that displays a list of mix suggestions in a scrollable view
 */
class SuggestionListComponent : public juce::Component,
                               public juce::ListBoxModel
{
public:
    SuggestionListComponent();
    ~SuggestionListComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Set the suggestion manager to display
    void setSuggestionManager(SuggestionManager* manager);
    
    // Filter suggestions by stem name
    void filterByStem(const juce::String& stemName);
    
    // Filter suggestions by type
    void filterByType(SuggestionType type);
    
    // Clear all filters
    void clearFilters();
    
    // ListBoxModel overrides
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    juce::Component* refreshComponentForRow(int rowNumber, bool isRowSelected, juce::Component* existingComponentToUpdate) override;
    void listBoxItemClicked(int row, const juce::MouseEvent& e) override;
    void selectedRowsChanged(int lastRowSelected) override;
    
    // Callback when a suggestion is selected
    std::function<void(const MixSuggestion&)> onSuggestionSelected;
    
    // Callback when a suggestion is double-clicked (to apply it)
    std::function<void(const MixSuggestion&)> onSuggestionDoubleClicked;

private:
    SuggestionManager* suggestionManager = nullptr;
    juce::ListBox suggestionListBox;
    
    // Filtered suggestions
    juce::Array<MixSuggestion> filteredSuggestions;
    
    // Current filters
    juce::String currentStemFilter;
    SuggestionType currentTypeFilter = SuggestionType::General;
    bool filtersActive = false;
    
    // Update the filtered suggestions based on current filters
    void updateFilteredSuggestions();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SuggestionListComponent)
};

} // namespace ForensEQ
