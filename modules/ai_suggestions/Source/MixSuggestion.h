#pragma once

#include <JuceHeader.h>

namespace ForensEQ {

/**
 * Enum representing the different types of suggestions that can be generated
 */
enum class SuggestionType {
    EQBalance,      // Suggestions related to frequency balance
    Loudness,       // Suggestions related to loudness levels
    StereoWidth,    // Suggestions related to stereo image
    Dynamics,       // Suggestions related to dynamic range
    General         // General mix suggestions
};

/**
 * Enum representing the severity or importance of a suggestion
 */
enum class SuggestionPriority {
    Low,            // Minor suggestion, small improvement
    Medium,         // Moderate suggestion, noticeable improvement
    High            // Critical suggestion, significant improvement
};

/**
 * Class representing a single mix suggestion
 */
class MixSuggestion {
public:
    MixSuggestion();
    
    MixSuggestion(const juce::String& stemName, 
                 SuggestionType type,
                 const juce::String& description,
                 SuggestionPriority priority = SuggestionPriority::Medium);
    
    ~MixSuggestion() = default;
    
    // Getters
    juce::String getStemName() const { return stemName; }
    SuggestionType getType() const { return type; }
    juce::String getDescription() const { return description; }
    SuggestionPriority getPriority() const { return priority; }
    bool isApplied() const { return applied; }
    
    // Setters
    void setStemName(const juce::String& name) { stemName = name; }
    void setType(SuggestionType t) { type = t; }
    void setDescription(const juce::String& desc) { description = desc; }
    void setPriority(SuggestionPriority p) { priority = p; }
    void setApplied(bool a) { applied = a; }
    
    // Convert suggestion type to string
    static juce::String suggestionTypeToString(SuggestionType type);
    
    // Convert suggestion priority to string
    static juce::String suggestionPriorityToString(SuggestionPriority priority);
    
    // Get color for this suggestion based on type and priority
    juce::Colour getColor() const;
    
private:
    juce::String stemName;         // Name of the stem this suggestion applies to
    SuggestionType type;           // Type of suggestion
    juce::String description;      // Textual description of the suggestion
    SuggestionPriority priority;   // Priority/importance of the suggestion
    bool applied = false;          // Whether this suggestion has been applied
};

} // namespace ForensEQ
