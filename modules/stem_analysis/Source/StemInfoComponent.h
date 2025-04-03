#pragma once

#include <JuceHeader.h>
#include "StemManager.h"

namespace ForensEQ {

/**
 * Component for displaying stem analysis information
 */
class StemInfoComponent : public juce::Component,
                         public juce::ChangeListener
{
public:
    StemInfoComponent(StemManager& stemManager);
    ~StemInfoComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // ChangeListener implementation
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    // Update the UI to reflect the current state
    void updateUI();

private:
    StemManager& stemManager;
    
    // Information labels
    juce::Label titleLabel;
    juce::Label stemTypeLabel;
    juce::Label lufsLabel;
    juce::Label rmsLabel;
    juce::Label widthLabel;
    juce::Label comparisonLabel;
    
    // UI colors
    juce::Colour backgroundColor = juce::Colour(30, 30, 30);
    juce::Colour textColor = juce::Colour(220, 220, 220);
    juce::Colour highlightColor = juce::Colour(0, 120, 255);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StemInfoComponent)
};

} // namespace ForensEQ
