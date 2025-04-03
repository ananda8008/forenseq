#include "EQVisualizerComponent.h"
#include "EQVisualizerControls.h"

namespace ForensEQ {

// Add these methods to the EQVisualizerComponent class
void EQVisualizerComponent::setParticleRate(float rate)
{
    // Clamp rate between 0.0 and 1.0
    float clampedRate = juce::jlimit(0.0f, 1.0f, rate);
    
    // This will affect how many particles are generated in timerCallback
    // We'll use this value in the random check
    particleGenerationRate = clampedRate;
}

void EQVisualizerComponent::setGlowIntensity(float intensity)
{
    // Clamp intensity between 0.0 and 1.0
    float clampedIntensity = juce::jlimit(0.0f, 1.0f, intensity);
    
    // This will affect the glow effect in drawOverlapHighlights
    glowIntensity = clampedIntensity;
}

} // namespace ForensEQ
