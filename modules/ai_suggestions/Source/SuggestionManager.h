#pragma once

#include <JuceHeader.h>
#include "MixSuggestion.h"

namespace ForensEQ {

/**
 * Class that manages a collection of mix suggestions
 */
class SuggestionManager {
public:
    SuggestionManager();
    ~SuggestionManager();
    
    // Add a suggestion to the collection
    void addSuggestion(const MixSuggestion& suggestion);
    
    // Clear all suggestions
    void clearSuggestions();
    
    // Get all suggestions
    const juce::Array<MixSuggestion>& getAllSuggestions() const;
    
    // Get suggestions for a specific stem
    juce::Array<MixSuggestion> getSuggestionsForStem(const juce::String& stemName) const;
    
    // Get suggestions of a specific type
    juce::Array<MixSuggestion> getSuggestionsOfType(SuggestionType type) const;
    
    // Get suggestions with a specific priority
    juce::Array<MixSuggestion> getSuggestionsWithPriority(SuggestionPriority priority) const;
    
    // Mark a suggestion as applied
    void markSuggestionAsApplied(int index, bool applied = true);
    
    // Get the number of suggestions
    int getNumSuggestions() const;
    
    // Get a specific suggestion by index
    const MixSuggestion& getSuggestion(int index) const;
    
    // Get a reference to a specific suggestion by index (for modification)
    MixSuggestion& getSuggestionReference(int index);
    
    // Sort suggestions by priority (highest first)
    void sortByPriority();
    
    // Sort suggestions by stem name
    void sortByStem();
    
    // Sort suggestions by type
    void sortByType();
    
private:
    juce::Array<MixSuggestion> suggestions;
};

} // namespace ForensEQ
