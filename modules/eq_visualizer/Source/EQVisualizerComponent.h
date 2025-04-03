#pragma once

#include <JuceHeader.h>

namespace ForensEQ {

/**
 * EQVisualizerComponent - A dual EQ visualization component that displays
 * both the user's mix and a reference track in a single view with visual effects.
 */
class EQVisualizerComponent : public juce::Component,
                             public juce::Timer
{
public:
    EQVisualizerComponent();
    ~EQVisualizerComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

    // Set the EQ data for the user's mix
    void setUserEQData(const std::vector<float>& frequencies, const std::vector<float>& magnitudes);
    
    // Set the EQ data for the reference track
    void setReferenceEQData(const std::vector<float>& frequencies, const std::vector<float>& magnitudes);
    
    // Toggle which curve appears on top
    void toggleCurveOrder();
    
    // Get current state of which curve is on top
    bool isUserCurveOnTop() const { return userCurveOnTop; }

private:
    // Curve data
    std::vector<float> userFrequencies;
    std::vector<float> userMagnitudes;
    std::vector<float> referenceFrequencies;
    std::vector<float> referenceMagnitudes;
    
    // Colors
    juce::Colour userCurveColor = juce::Colour(0, 120, 255);      // Blue
    juce::Colour referenceCurveColor = juce::Colour(255, 120, 0); // Orange
    juce::Colour overlapHighlightColor = juce::Colour(255, 255, 200); // Light gold
    juce::Colour backgroundColor = juce::Colour(30, 30, 30);      // Dark charcoal
    juce::Colour textColor = juce::Colour(220, 220, 220);         // Light gray
    
    // Display settings
    bool userCurveOnTop = true;
    float curveThickness = 2.0f;
    float overlapThreshold = 3.0f; // dB threshold to consider curves as "overlapping"
    
    // Animation properties
    struct Particle {
        float x, y;
        float vx, vy;
        float life;
        float maxLife;
        juce::Colour color;
    };
    
    std::vector<Particle> particles;
    juce::Random random;
    float particleGenerationRate = 0.3f;
    float glowIntensity = 0.7f;
    
    // Methods for controlling visualization settings
    void setParticleRate(float rate);
    void setGlowIntensity(float intensity);
    
    // Helper methods
    void drawBackground(juce::Graphics& g);
    void drawFrequencyLabels(juce::Graphics& g);
    void drawEQCurve(juce::Graphics& g, const std::vector<float>& frequencies, 
                    const std::vector<float>& magnitudes, juce::Colour color);
    void drawOverlapHighlights(juce::Graphics& g);
    void updateParticles();
    void addParticlesAlongCurve(const std::vector<float>& frequencies, 
                               const std::vector<float>& magnitudes, juce::Colour color);
    
    // Convert frequency to x position
    float freqToX(float freq) const;
    
    // Convert magnitude (dB) to y position
    float dbToY(float db) const;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQVisualizerComponent)
};

} // namespace ForensEQ
