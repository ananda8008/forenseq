#include "SuggestionManager.h"

namespace ForensEQ {

SuggestionManager::SuggestionManager()
{
}

SuggestionManager::~SuggestionManager()
{
}

void SuggestionManager::addSuggestion(const MixSuggestion& suggestion)
{
    suggestions.add(suggestion);
}

void SuggestionManager::clearSuggestions()
{
    suggestions.clear();
}

const juce::Array<MixSuggestion>& SuggestionManager::getAllSuggestions() const
{
    return suggestions;
}

juce::Array<MixSuggestion> SuggestionManager::getSuggestionsForStem(const juce::String& stemName) const
{
    juce::Array<MixSuggestion> result;
    
    for (const auto& suggestion : suggestions)
    {
        if (suggestion.getStemName() == stemName)
        {
            result.add(suggestion);
        }
    }
    
    return result;
}

juce::Array<MixSuggestion> SuggestionManager::getSuggestionsOfType(SuggestionType type) const
{
    juce::Array<MixSuggestion> result;
    
    for (const auto& suggestion : suggestions)
    {
        if (suggestion.getType() == type)
        {
            result.add(suggestion);
        }
    }
    
    return result;
}

juce::Array<MixSuggestion> SuggestionManager::getSuggestionsWithPriority(SuggestionPriority priority) const
{
    juce::Array<MixSuggestion> result;
    
    for (const auto& suggestion : suggestions)
    {
        if (suggestion.getPriority() == priority)
        {
            result.add(suggestion);
        }
    }
    
    return result;
}

void SuggestionManager::markSuggestionAsApplied(int index, bool applied)
{
    if (index >= 0 && index < suggestions.size())
    {
        suggestions.getReference(index).setApplied(applied);
    }
}

int SuggestionManager::getNumSuggestions() const
{
    return suggestions.size();
}

const MixSuggestion& SuggestionManager::getSuggestion(int index) const
{
    return suggestions[index];
}

MixSuggestion& SuggestionManager::getSuggestionReference(int index)
{
    return suggestions.getReference(index);
}

void SuggestionManager::sortByPriority()
{
    suggestions.sort([](const MixSuggestion& a, const MixSuggestion& b) {
        // Sort by priority (high to low)
        return static_cast<int>(b.getPriority()) < static_cast<int>(a.getPriority());
    });
}

void SuggestionManager::sortByStem()
{
    suggestions.sort([](const MixSuggestion& a, const MixSuggestion& b) {
        // Sort by stem name (alphabetically)
        return a.getStemName().compareIgnoreCase(b.getStemName()) < 0;
    });
}

void SuggestionManager::sortByType()
{
    suggestions.sort([](const MixSuggestion& a, const MixSuggestion& b) {
        // Sort by suggestion type
        return static_cast<int>(a.getType()) < static_cast<int>(b.getType());
    });
}

} // namespace ForensEQ
