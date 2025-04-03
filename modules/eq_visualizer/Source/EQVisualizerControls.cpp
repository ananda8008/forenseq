#include "EQVisualizerControls.h"

namespace ForensEQ {

EQVisualizerControls::EQVisualizerControls(EQVisualizerComponent& visualizer)
    : eqVisualizer(visualizer)
{
    // Set up toggle button
    toggleCurveOrderButton.setButtonText("Toggle Curve Order");
    toggleCurveOrderButton.onClick = [this] { 
        eqVisualizer.toggleCurveOrder(); 
        toggleCurveOrderButton.setButtonText(eqVisualizer.isUserCurveOnTop() ? 
                                          "User Curve on Top (Click to Toggle)" : 
                                          "Reference Curve on Top (Click to Toggle)");
    };
    addAndMakeVisible(toggleCurveOrderButton);
    
    // Set up user EQ group
    userEQGroup.setText("User Mix EQ");
    addAndMakeVisible(userEQGroup);
    
    // Set up reference EQ group
    referenceEQGroup.setText("Reference Track EQ");
    addAndMakeVisible(referenceEQGroup);
    
    // Set up visualization group
    visualizationGroup.setText("Visualization Settings");
    addAndMakeVisible(visualizationGroup);
    
    // Set up user EQ sliders
    setupSlider(userBassSlider, userBassLabel, "Bass", -12.0, 12.0, 0.0);
    setupSlider(userMidSlider, userMidLabel, "Mid", -12.0, 12.0, 0.0);
    setupSlider(userTrebleSlider, userTrebleLabel, "Treble", -12.0, 12.0, 0.0);
    
    // Set up reference EQ sliders
    setupSlider(refBassSlider, refBassLabel, "Bass", -12.0, 12.0, 0.0);
    setupSlider(refMidSlider, refMidLabel, "Mid", -12.0, 12.0, 0.0);
    setupSlider(refTrebleSlider, refTrebleLabel, "Treble", -12.0, 12.0, 0.0);
    
    // Set up visualization sliders
    setupSlider(particleRateSlider, particleRateLabel, "Particle Rate", 0.0, 1.0, 0.3);
    setupSlider(glowIntensitySlider, glowIntensityLabel, "Glow Intensity", 0.0, 1.0, 0.7);
    
    // Set up slider listeners
    userBassSlider.onValueChange = [this] { updateUserEQCurve(); };
    userMidSlider.onValueChange = [this] { updateUserEQCurve(); };
    userTrebleSlider.onValueChange = [this] { updateUserEQCurve(); };
    
    refBassSlider.onValueChange = [this] { updateReferenceEQCurve(); };
    refMidSlider.onValueChange = [this] { updateReferenceEQCurve(); };
    refTrebleSlider.onValueChange = [this] { updateReferenceEQCurve(); };
    
    particleRateSlider.onValueChange = [this] { updateVisualizationSettings(); };
    glowIntensitySlider.onValueChange = [this] { updateVisualizationSettings(); };
    
    // Initialize with default values
    updateUserEQCurve();
    updateReferenceEQCurve();
    updateVisualizationSettings();
}

EQVisualizerControls::~EQVisualizerControls()
{
}

void EQVisualizerControls::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(40, 40, 40));
}

void EQVisualizerControls::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    
    // Position the toggle button at the top
    toggleCurveOrderButton.setBounds(bounds.removeFromTop(30).reduced(5, 0));
    
    // Add some spacing
    bounds.removeFromTop(10);
    
    // Divide the remaining space into three columns
    auto userEQBounds = bounds.removeFromLeft(bounds.getWidth() / 3).reduced(5);
    auto refEQBounds = bounds.removeFromLeft(bounds.getWidth() / 2).reduced(5);
    auto visualizationBounds = bounds.reduced(5);
    
    // Set up user EQ group
    userEQGroup.setBounds(userEQBounds);
    auto userEQContent = userEQBounds.reduced(10);
    
    // Position user EQ sliders
    userBassSlider.setBounds(userEQContent.removeFromLeft(userEQContent.getWidth() / 3).reduced(5));
    userMidSlider.setBounds(userEQContent.removeFromLeft(userEQContent.getWidth() / 2).reduced(5));
    userTrebleSlider.setBounds(userEQContent.reduced(5));
    
    // Set up reference EQ group
    referenceEQGroup.setBounds(refEQBounds);
    auto refEQContent = refEQBounds.reduced(10);
    
    // Position reference EQ sliders
    refBassSlider.setBounds(refEQContent.removeFromLeft(refEQContent.getWidth() / 3).reduced(5));
    refMidSlider.setBounds(refEQContent.removeFromLeft(refEQContent.getWidth() / 2).reduced(5));
    refTrebleSlider.setBounds(refEQContent.reduced(5));
    
    // Set up visualization group
    visualizationGroup.setBounds(visualizationBounds);
    auto visualizationContent = visualizationBounds.reduced(10);
    
    // Position visualization sliders
    particleRateSlider.setBounds(visualizationContent.removeFromLeft(visualizationContent.getWidth() / 2).reduced(5));
    glowIntensitySlider.setBounds(visualizationContent.reduced(5));
}

void EQVisualizerControls::setParamChangeCallback(ParamChangeCallback callback)
{
    paramChangeCallback = std::move(callback);
}

void EQVisualizerControls::setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& text, 
                                     double min, double max, double initial)
{
    slider.setRange(min, max);
    slider.setValue(initial);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    slider.setSliderStyle(juce::Slider::LinearVertical);
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::lightblue);
    slider.setColour(juce::Slider::trackColourId, juce::Colours::darkgrey);
    
    label.setText(text, juce::dontSendNotification);
    label.attachToComponent(&slider, false);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::white);
    
    addAndMakeVisible(slider);
    addAndMakeVisible(label);
}

void EQVisualizerControls::updateUserEQCurve()
{
    // Create frequency array
    const int numPoints = 100;
    std::vector<float> frequencies(numPoints);
    std::vector<float> magnitudes(numPoints);
    
    for (int i = 0; i < numPoints; ++i)
    {
        float t = static_cast<float>(i) / static_cast<float>(numPoints - 1);
        frequencies[i] = 20.0f * std::pow(1000.0f, t);
        
        // Apply EQ curve based on slider values
        float freq = frequencies[i];
        float bassGain = static_cast<float>(userBassSlider.getValue());
        float midGain = static_cast<float>(userMidSlider.getValue());
        float trebleGain = static_cast<float>(userTrebleSlider.getValue());
        
        // Simple 3-band EQ model
        if (freq < 250.0f)
            magnitudes[i] = bassGain;
        else if (freq < 4000.0f)
            magnitudes[i] = midGain;
        else
            magnitudes[i] = trebleGain;
            
        // Smooth transitions between bands
        if (freq > 200.0f && freq < 300.0f)
        {
            float t = (freq - 200.0f) / 100.0f;
            magnitudes[i] = bassGain * (1.0f - t) + midGain * t;
        }
        else if (freq > 3000.0f && freq < 5000.0f)
        {
            float t = (freq - 3000.0f) / 2000.0f;
            magnitudes[i] = midGain * (1.0f - t) + trebleGain * t;
        }
    }
    
    // Update the visualizer
    eqVisualizer.setUserEQData(frequencies, magnitudes);
    
    // Call the callback if set
    if (paramChangeCallback)
        paramChangeCallback();
}

void EQVisualizerControls::updateReferenceEQCurve()
{
    // Create frequency array
    const int numPoints = 100;
    std::vector<float> frequencies(numPoints);
    std::vector<float> magnitudes(numPoints);
    
    for (int i = 0; i < numPoints; ++i)
    {
        float t = static_cast<float>(i) / static_cast<float>(numPoints - 1);
        frequencies[i] = 20.0f * std::pow(1000.0f, t);
        
        // Apply EQ curve based on slider values
        float freq = frequencies[i];
        float bassGain = static_cast<float>(refBassSlider.getValue());
        float midGain = static_cast<float>(refMidSlider.getValue());
        float trebleGain = static_cast<float>(refTrebleSlider.getValue());
        
        // Simple 3-band EQ model
        if (freq < 250.0f)
            magnitudes[i] = bassGain;
        else if (freq < 4000.0f)
            magnitudes[i] = midGain;
        else
            magnitudes[i] = trebleGain;
            
        // Smooth transitions between bands
        if (freq > 200.0f && freq < 300.0f)
        {
            float t = (freq - 200.0f) / 100.0f;
            magnitudes[i] = bassGain * (1.0f - t) + midGain * t;
        }
        else if (freq > 3000.0f && freq < 5000.0f)
        {
            float t = (freq - 3000.0f) / 2000.0f;
            magnitudes[i] = midGain * (1.0f - t) + trebleGain * t;
        }
    }
    
    // Update the visualizer
    eqVisualizer.setReferenceEQData(frequencies, magnitudes);
    
    // Call the callback if set
    if (paramChangeCallback)
        paramChangeCallback();
}

void EQVisualizerControls::updateVisualizationSettings()
{
    // Update the visualization settings in the EQ visualizer
    float particleRate = static_cast<float>(particleRateSlider.getValue());
    float glowIntensity = static_cast<float>(glowIntensitySlider.getValue());
    
    eqVisualizer.setParticleRate(particleRate);
    eqVisualizer.setGlowIntensity(glowIntensity);
    
    // Call the callback if set
    if (paramChangeCallback)
        paramChangeCallback();
}

} // namespace ForensEQ
