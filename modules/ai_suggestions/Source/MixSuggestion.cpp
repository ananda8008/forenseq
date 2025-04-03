#include "MixSuggestion.h"

namespace ForensEQ {

MixSuggestion::MixSuggestion()
    : stemName("Full Mix"),
      type(SuggestionType::General),
      description("No suggestion available"),
      priority(SuggestionPriority::Low),
      applied(false)
{
}

MixSuggestion::MixSuggestion(const juce::String& stemName, 
                           SuggestionType type,
                           const juce::String& description,
                           SuggestionPriority priority)
    : stemName(stemName),
      type(type),
      description(description),
      priority(priority),
      applied(false)
{
}

juce::String MixSuggestion::suggestionTypeToString(SuggestionType type)
{
    switch (type)
    {
        case SuggestionType::EQBalance:
            return "EQ Balance";
        case SuggestionType::Loudness:
            return "Loudness";
        case SuggestionType::StereoWidth:
            return "Stereo Width";
        case SuggestionType::Dynamics:
            return "Dynamics";
        case SuggestionType::General:
            return "General";
        default:
            return "Unknown";
    }
}

juce::String MixSuggestion::suggestionPriorityToString(SuggestionPriority priority)
{
    switch (priority)
    {
        case SuggestionPriority::Low:
            return "Low";
        case SuggestionPriority::Medium:
            return "Medium";
        case SuggestionPriority::High:
            return "High";
        default:
            return "Unknown";
    }
}

juce::Colour MixSuggestion::getColor() const
{
    // Base colors for each suggestion type
    juce::Colour baseColor;
    
    switch (type)
    {
        case SuggestionType::EQBalance:
            baseColor = juce::Colour(0, 150, 255);  // Blue for EQ
            break;
        case SuggestionType::Loudness:
            baseColor = juce::Colour(255, 100, 0);  // Orange for Loudness
            break;
        case SuggestionType::StereoWidth:
            baseColor = juce::Colour(0, 200, 100);  // Green for Stereo Width
            break;
        case SuggestionType::Dynamics:
            baseColor = juce::Colour(200, 0, 200);  // Purple for Dynamics
            break;
        case SuggestionType::General:
            baseColor = juce::Colour(200, 200, 200); // Gray for General
            break;
        default:
            baseColor = juce::Colour(150, 150, 150);
    }
    
    // Adjust brightness based on priority
    float brightness = 1.0f;
    
    switch (priority)
    {
        case SuggestionPriority::Low:
            brightness = 0.7f;
            break;
        case SuggestionPriority::Medium:
            brightness = 1.0f;
            break;
        case SuggestionPriority::High:
            brightness = 1.3f;
            break;
    }
    
    // Apply brightness adjustment
    juce::Colour adjustedColor = baseColor.brighter(brightness);
    
    // If the suggestion has been applied, make it more transparent
    if (applied)
    {
        return adjustedColor.withAlpha(0.5f);
    }
    
    return adjustedColor;
}

} // namespace ForensEQ
