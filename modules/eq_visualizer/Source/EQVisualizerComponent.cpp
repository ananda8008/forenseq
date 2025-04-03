#include "EQVisualizerComponent.h"

namespace ForensEQ {

EQVisualizerComponent::EQVisualizerComponent()
{
    // Initialize with some default frequencies (20Hz to 20kHz)
    const int numPoints = 100;
    userFrequencies.resize(numPoints);
    userMagnitudes.resize(numPoints, 0.0f);
    referenceFrequencies.resize(numPoints);
    referenceMagnitudes.resize(numPoints, 0.0f);
    
    // Generate logarithmic frequency scale
    for (int i = 0; i < numPoints; ++i)
    {
        float t = static_cast<float>(i) / static_cast<float>(numPoints - 1);
        userFrequencies[i] = 20.0f * std::pow(1000.0f, t);
        referenceFrequencies[i] = userFrequencies[i];
    }
    
    // Start the timer for animations
    startTimerHz(60); // 60 fps for smooth animations
    
    // Set component properties
    setOpaque(true);
    setBufferedToImage(true);
}

EQVisualizerComponent::~EQVisualizerComponent()
{
    stopTimer();
}

void EQVisualizerComponent::paint(juce::Graphics& g)
{
    // Draw the background
    drawBackground(g);
    
    // Draw frequency labels
    drawFrequencyLabels(g);
    
    // Draw EQ curves in the correct order
    if (userCurveOnTop)
    {
        drawEQCurve(g, referenceFrequencies, referenceMagnitudes, referenceCurveColor);
        drawOverlapHighlights(g);
        drawEQCurve(g, userFrequencies, userMagnitudes, userCurveColor);
    }
    else
    {
        drawEQCurve(g, userFrequencies, userMagnitudes, userCurveColor);
        drawOverlapHighlights(g);
        drawEQCurve(g, referenceFrequencies, referenceMagnitudes, referenceCurveColor);
    }
    
    // Draw particles
    for (const auto& particle : particles)
    {
        g.setColour(particle.color.withAlpha(particle.life / particle.maxLife));
        g.fillEllipse(particle.x - 2.0f, particle.y - 2.0f, 4.0f, 4.0f);
    }
}

void EQVisualizerComponent::resized()
{
    // Clear particles when resizing
    particles.clear();
}

void EQVisualizerComponent::timerCallback()
{
    // Update particles
    updateParticles();
    
    // Add new particles along the curves
    if (random.nextFloat() < particleGenerationRate) // Use the configurable rate
    {
        addParticlesAlongCurve(userFrequencies, userMagnitudes, userCurveColor);
        addParticlesAlongCurve(referenceFrequencies, referenceMagnitudes, referenceCurveColor);
    }
    
    // Trigger repaint
    repaint();
}

void EQVisualizerComponent::setUserEQData(const std::vector<float>& frequencies, const std::vector<float>& magnitudes)
{
    if (frequencies.size() == magnitudes.size() && !frequencies.empty())
    {
        userFrequencies = frequencies;
        userMagnitudes = magnitudes;
        repaint();
    }
}

void EQVisualizerComponent::setReferenceEQData(const std::vector<float>& frequencies, const std::vector<float>& magnitudes)
{
    if (frequencies.size() == magnitudes.size() && !frequencies.empty())
    {
        referenceFrequencies = frequencies;
        referenceMagnitudes = magnitudes;
        repaint();
    }
}

void EQVisualizerComponent::toggleCurveOrder()
{
    userCurveOnTop = !userCurveOnTop;
    repaint();
}

void EQVisualizerComponent::drawBackground(juce::Graphics& g)
{
    const auto bounds = getLocalBounds().toFloat();
    
    // Fill background with gradient
    g.setGradientFill(juce::ColourGradient(
        backgroundColor.darker(0.2f), bounds.getX(), bounds.getY(),
        backgroundColor, bounds.getX(), bounds.getBottom(),
        false));
    g.fillRect(bounds);
    
    // Draw grid lines
    g.setColour(textColor.withAlpha(0.2f));
    
    // Vertical grid lines (frequency)
    const float frequencies[] = { 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000 };
    for (float freq : frequencies)
    {
        float x = freqToX(freq);
        g.drawVerticalLine(static_cast<int>(x), bounds.getY(), bounds.getBottom());
    }
    
    // Horizontal grid lines (dB)
    for (int db = -24; db <= 24; db += 6)
    {
        float y = dbToY(static_cast<float>(db));
        g.drawHorizontalLine(static_cast<int>(y), bounds.getX(), bounds.getRight());
    }
}

void EQVisualizerComponent::drawFrequencyLabels(juce::Graphics& g)
{
    const auto bounds = getLocalBounds().toFloat();
    g.setColour(textColor);
    g.setFont(15.0f);
    
    // Draw frequency band labels
    struct FreqBand { float freq; const char* name; };
    const FreqBand bands[] = {
        { 30.0f, "Sub" },
        { 100.0f, "Low" },
        { 500.0f, "Mid" },
        { 3000.0f, "High-Mid" },
        { 10000.0f, "High" }
    };
    
    for (const auto& band : bands)
    {
        float x = freqToX(band.freq);
        g.drawText(band.name, static_cast<int>(x - 30), static_cast<int>(bounds.getBottom() - 25), 
                  60, 20, juce::Justification::centred);
    }
    
    // Draw dB scale
    g.setFont(12.0f);
    for (int db = -24; db <= 24; db += 6)
    {
        if (db == 0) continue; // Skip 0 dB for cleaner look
        
        float y = dbToY(static_cast<float>(db));
        g.drawText(juce::String(db) + " dB", 5, static_cast<int>(y - 10), 40, 20, 
                  juce::Justification::left);
    }
}

void EQVisualizerComponent::drawEQCurve(juce::Graphics& g, const std::vector<float>& frequencies, 
                                      const std::vector<float>& magnitudes, juce::Colour color)
{
    if (frequencies.empty() || magnitudes.empty() || frequencies.size() != magnitudes.size())
        return;
    
    // Create path for the curve
    juce::Path curvePath;
    bool pathStarted = false;
    
    for (size_t i = 0; i < frequencies.size(); ++i)
    {
        float x = freqToX(frequencies[i]);
        float y = dbToY(magnitudes[i]);
        
        if (!pathStarted)
        {
            curvePath.startNewSubPath(x, y);
            pathStarted = true;
        }
        else
        {
            curvePath.lineTo(x, y);
        }
    }
    
    // Draw the curve
    g.setColour(color);
    g.strokePath(curvePath, juce::PathStrokeType(curveThickness));
}

void EQVisualizerComponent::drawOverlapHighlights(juce::Graphics& g)
{
    if (userFrequencies.empty() || referenceFrequencies.empty())
        return;
    
    // Create a path for the overlap highlights
    juce::Path overlapPath;
    bool pathStarted = false;
    
    // We'll need to interpolate between the two frequency sets
    // For simplicity, we'll use the user frequencies as the base
    for (size_t i = 0; i < userFrequencies.size(); ++i)
    {
        float freq = userFrequencies[i];
        float userMag = userMagnitudes[i];
        
        // Find the closest reference magnitude for this frequency
        float refMag = 0.0f;
        for (size_t j = 0; j < referenceFrequencies.size(); ++j)
        {
            if (std::abs(referenceFrequencies[j] - freq) < 0.01f * freq)
            {
                refMag = referenceMagnitudes[j];
                break;
            }
        }
        
        // Check if the magnitudes are close enough to be considered "overlapping"
        if (std::abs(userMag - refMag) <= overlapThreshold)
        {
            float x = freqToX(freq);
            float y = dbToY(userMag);
            
            if (!pathStarted)
            {
                overlapPath.startNewSubPath(x, y);
                pathStarted = true;
            }
            else
            {
                overlapPath.lineTo(x, y);
            }
        }
        else if (pathStarted)
        {
            pathStarted = false;
        }
    }
    
    // Draw the overlap highlights with a glow effect
    if (!overlapPath.isEmpty())
    {
        // Create a glow effect by drawing multiple strokes with decreasing alpha
        // Use glowIntensity to control the effect
        int numLayers = static_cast<int>(5.0f + glowIntensity * 5.0f); // 5-10 layers based on intensity
        
        for (int i = numLayers; i >= 0; --i)
        {
            float alpha = glowIntensity * 0.7f * (1.0f - static_cast<float>(i) / static_cast<float>(numLayers));
            float thickness = curveThickness + i * (1.0f + glowIntensity * 1.0f);
            
            g.setColour(overlapHighlightColor.withAlpha(alpha));
            g.strokePath(overlapPath, juce::PathStrokeType(thickness));
        }
    }
}

void EQVisualizerComponent::updateParticles()
{
    // Update existing particles
    for (auto it = particles.begin(); it != particles.end(); )
    {
        it->x += it->vx;
        it->y += it->vy;
        it->life -= 0.01f;
        
        if (it->life <= 0.0f)
        {
            it = particles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void EQVisualizerComponent::addParticlesAlongCurve(const std::vector<float>& frequencies, 
                                                 const std::vector<float>& magnitudes, 
                                                 juce::Colour color)
{
    if (frequencies.empty() || magnitudes.empty())
        return;
    
    // Add particles at random points along the curve
    int index = random.nextInt(static_cast<int>(frequencies.size()));
    float x = freqToX(frequencies[index]);
    float y = dbToY(magnitudes[index]);
    
    // Create a new particle
    Particle p;
    p.x = x;
    p.y = y;
    p.vx = random.nextFloat() * 0.6f - 0.3f;
    p.vy = random.nextFloat() * 0.6f - 0.3f;
    p.life = 1.0f;
    p.maxLife = 1.0f;
    p.color = color.brighter(0.5f);
    
    // Add to the particle list (limit total particles for performance)
    if (particles.size() < 200)
    {
        particles.push_back(p);
    }
}

float EQVisualizerComponent::freqToX(float freq) const
{
    // Convert frequency to x position using logarithmic scale
    const auto bounds = getLocalBounds().toFloat();
    const float minFreq = 20.0f;
    const float maxFreq = 20000.0f;
    
    float normX = std::log10(freq / minFreq) / std::log10(maxFreq / minFreq);
    return bounds.getX() + normX * bounds.getWidth();
}

float EQVisualizerComponent::dbToY(float db) const
{
    // Convert dB to y position
    const auto bounds = getLocalBounds().toFloat();
    const float minDb = -24.0f;
    const float maxDb = 24.0f;
    
    float normY = 1.0f - (db - minDb) / (maxDb - minDb);
    return bounds.getY() + normY * bounds.getHeight() * 0.8f + bounds.getHeight() * 0.1f;
}

} // namespace ForensEQ
