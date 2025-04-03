#include "SuggestionListComponent.h"

namespace ForensEQ {

SuggestionListComponent::SuggestionListComponent()
{
    // Set up the list box
    suggestionListBox.setModel(this);
    suggestionListBox.setRowHeight(70);
    suggestionListBox.setMultipleSelectionEnabled(false);
    suggestionListBox.setColour(juce::ListBox::backgroundColourId, juce::Colour(30, 30, 30));
    suggestionListBox.setColour(juce::ListBox::outlineColourId, juce::Colour(60, 60, 60));
    
    addAndMakeVisible(suggestionListBox);
}

SuggestionListComponent::~SuggestionListComponent()
{
}

void SuggestionListComponent::paint(juce::Graphics& g)
{
    // Fill background
    g.fillAll(juce::Colour(25, 25, 25));
}

void SuggestionListComponent::resized()
{
    // Make the list box fill the entire component
    suggestionListBox.setBounds(getLocalBounds());
}

void SuggestionListComponent::setSuggestionManager(SuggestionManager* manager)
{
    suggestionManager = manager;
    updateFilteredSuggestions();
    suggestionListBox.updateContent();
    suggestionListBox.repaint();
}

void SuggestionListComponent::filterByStem(const juce::String& stemName)
{
    currentStemFilter = stemName;
    filtersActive = !stemName.isEmpty();
    updateFilteredSuggestions();
    suggestionListBox.updateContent();
    suggestionListBox.repaint();
}

void SuggestionListComponent::filterByType(SuggestionType type)
{
    currentTypeFilter = type;
    filtersActive = true;
    updateFilteredSuggestions();
    suggestionListBox.updateContent();
    suggestionListBox.repaint();
}

void SuggestionListComponent::clearFilters()
{
    currentStemFilter = "";
    filtersActive = false;
    updateFilteredSuggestions();
    suggestionListBox.updateContent();
    suggestionListBox.repaint();
}

int SuggestionListComponent::getNumRows()
{
    if (suggestionManager == nullptr)
        return 0;
    
    return filtersActive ? filteredSuggestions.size() : suggestionManager->getNumSuggestions();
}

void SuggestionListComponent::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    // This is not used as we're using custom components for each row
}

juce::Component* SuggestionListComponent::refreshComponentForRow(int rowNumber, bool isRowSelected, juce::Component* existingComponentToUpdate)
{
    // Get the suggestion for this row
    MixSuggestion suggestion;
    
    if (rowNumber >= 0 && rowNumber < getNumRows())
    {
        if (filtersActive)
        {
            suggestion = filteredSuggestions[rowNumber];
        }
        else if (suggestionManager != nullptr)
        {
            suggestion = suggestionManager->getSuggestion(rowNumber);
        }
    }
    
    // Create or update the component
    SuggestionItemComponent* component = static_cast<SuggestionItemComponent*>(existingComponentToUpdate);
    
    if (component == nullptr)
    {
        component = new SuggestionItemComponent(suggestion);
    }
    else
    {
        component->updateSuggestion(suggestion);
    }
    
    component->setSelected(isRowSelected);
    
    return component;
}

void SuggestionListComponent::listBoxItemClicked(int row, const juce::MouseEvent& e)
{
    // Handle single click
    if (row >= 0 && row < getNumRows())
    {
        MixSuggestion suggestion;
        
        if (filtersActive)
        {
            suggestion = filteredSuggestions[row];
        }
        else if (suggestionManager != nullptr)
        {
            suggestion = suggestionManager->getSuggestion(row);
        }
        
        if (onSuggestionSelected)
        {
            onSuggestionSelected(suggestion);
        }
    }
}

void SuggestionListComponent::selectedRowsChanged(int lastRowSelected)
{
    // Handle selection change
    if (lastRowSelected >= 0 && lastRowSelected < getNumRows())
    {
        MixSuggestion suggestion;
        
        if (filtersActive)
        {
            suggestion = filteredSuggestions[lastRowSelected];
        }
        else if (suggestionManager != nullptr)
        {
            suggestion = suggestionManager->getSuggestion(lastRowSelected);
        }
        
        if (onSuggestionSelected)
        {
            onSuggestionSelected(suggestion);
        }
    }
}

void SuggestionListComponent::updateFilteredSuggestions()
{
    filteredSuggestions.clear();
    
    if (suggestionManager == nullptr)
        return;
    
    // Apply filters
    if (filtersActive)
    {
        for (int i = 0; i < suggestionManager->getNumSuggestions(); ++i)
        {
            const MixSuggestion& suggestion = suggestionManager->getSuggestion(i);
            
            bool matchesStem = currentStemFilter.isEmpty() || suggestion.getStemName() == currentStemFilter;
            bool matchesType = suggestion.getType() == currentTypeFilter;
            
            if (matchesStem && (matchesType || currentTypeFilter == SuggestionType::General))
            {
                filteredSuggestions.add(suggestion);
            }
        }
    }
    else
    {
        // No filters active, just use all suggestions
        for (int i = 0; i < suggestionManager->getNumSuggestions(); ++i)
        {
            filteredSuggestions.add(suggestionManager->getSuggestion(i));
        }
    }
}

} // namespace ForensEQ
