#pragma once

#include <JuceHeader.h>
#include "ForensEQTheme.h"

/**
 * ForensEQMainComponent - Main component class for the ForensEQ plugin
 * 
 * Integrates all modules into a unified interface with consistent styling
 */
class ForensEQMainComponent : public juce::Component,
                             public juce::Button::Listener
{
public:
    ForensEQMainComponent();
    ~ForensEQMainComponent() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    
    // Module visibility control
    void showModule(const juce::String& moduleName);
    
private:
    // Theme instance
    ForensEQTheme& theme = ForensEQTheme::getInstance();
    
    // Module navigation buttons
    std::unique_ptr<juce::TextButton> waveformViewerButton;
    std::unique_ptr<juce::TextButton> eqVisualizerButton;
    std::unique_ptr<juce::TextButton> stemAnalysisButton;
    std::unique_ptr<juce::TextButton> loudnessWidthButton;
    std::unique_ptr<juce::TextButton> aiSuggestionsButton;
    
    // Module components (will be loaded from respective modules)
    std::unique_ptr<juce::Component> waveformViewerComponent;
    std::unique_ptr<juce::Component> eqVisualizerComponent;
    std::unique_ptr<juce::Component> stemAnalysisComponent;
    std::unique_ptr<juce::Component> loudnessWidthComponent;
    std::unique_ptr<juce::Component> aiSuggestionsComponent;
    
    // Current active module
    juce::String currentModule;
    
    // Helper methods
    void createModuleComponents();
    void setupNavigationButtons();
    void applyThemeToAllComponents();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ForensEQMainComponent)
};
