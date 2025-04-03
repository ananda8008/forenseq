#pragma once

#include <JuceHeader.h>
#include "EQVisualizerComponent.h"

namespace ForensEQ {

/**
 * EQVisualizerControls - A component that provides UI controls for the EQ visualizer
 */
class EQVisualizerControls : public juce::Component
{
public:
    EQVisualizerControls(EQVisualizerComponent& visualizer);
    ~EQVisualizerControls() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Set callback for when EQ parameters change
    using ParamChangeCallback = std::function<void()>;
    void setParamChangeCallback(ParamChangeCallback callback);

private:
    EQVisualizerComponent& eqVisualizer;
    
    // UI Components
    juce::TextButton toggleCurveOrderButton;
    
    // User EQ controls
    juce::GroupComponent userEQGroup;
    juce::Slider userBassSlider, userMidSlider, userTrebleSlider;
    juce::Label userBassLabel, userMidLabel, userTrebleLabel;
    
    // Reference EQ controls
    juce::GroupComponent referenceEQGroup;
    juce::Slider refBassSlider, refMidSlider, refTrebleSlider;
    juce::Label refBassLabel, refMidLabel, refTrebleLabel;
    
    // Visualization controls
    juce::GroupComponent visualizationGroup;
    juce::Slider particleRateSlider, glowIntensitySlider;
    juce::Label particleRateLabel, glowIntensityLabel;
    
    // Callback
    ParamChangeCallback paramChangeCallback;
    
    // Helper methods
    void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& text, 
                    double min, double max, double initial);
    void updateUserEQCurve();
    void updateReferenceEQCurve();
    void updateVisualizationSettings();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQVisualizerControls)
};

} // namespace ForensEQ
