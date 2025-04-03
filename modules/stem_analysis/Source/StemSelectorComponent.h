#pragma once

#include <JuceHeader.h>
#include "StemManager.h"

namespace ForensEQ {

/**
 * Component for selecting and displaying stem types
 */
class StemSelectorComponent : public juce::Component,
                             public juce::Button::Listener,
                             public juce::ChangeListener
{
public:
    StemSelectorComponent(StemManager& stemManager);
    ~StemSelectorComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    
    // Button::Listener implementation
    void buttonClicked(juce::Button* button) override;
    
    // ChangeListener implementation
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    
    // Update the UI to reflect the current state
    void updateUI();

private:
    StemManager& stemManager;
    
    // Buttons for each stem type
    juce::TextButton fullMixButton{"Full Mix"};
    juce::TextButton kickButton{"Kick"};
    juce::TextButton snareButton{"Snare"};
    juce::TextButton bassButton{"Bass"};
    juce::TextButton vocalsButton{"Vocals"};
    juce::TextButton otherButton{"Other"};
    
    // Status label
    juce::Label statusLabel;
    
    // UI colors
    juce::Colour backgroundColor = juce::Colour(30, 30, 30);
    juce::Colour textColor = juce::Colour(220, 220, 220);
    juce::Colour selectedColor = juce::Colour(0, 120, 255);
    juce::Colour disabledColor = juce::Colour(60, 60, 60);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StemSelectorComponent)
};

} // namespace ForensEQ
