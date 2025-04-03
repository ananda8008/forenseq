#pragma once

#include <JuceHeader.h>

namespace ForensEQ {

/**
 * Enum representing the different types of stems that can be isolated
 */
enum class StemType {
    Kick,
    Snare,
    Bass,
    Vocals,
    Other,
    Full // The full mix
};

/**
 * Class representing the data model for a single stem
 */
class StemData {
public:
    StemData(StemType type = StemType::Full);
    ~StemData();
    
    // Get the name of this stem
    juce::String getName() const;
    
    // Get the type of this stem
    StemType getType() const;
    
    // Set/get the audio buffer for this stem
    void setAudioBuffer(const juce::AudioBuffer<float>& buffer);
    const juce::AudioBuffer<float>& getAudioBuffer() const;
    
    // Set/get the frequency data for this stem
    void setFrequencyData(const std::vector<float>& frequencies, const std::vector<float>& magnitudes);
    void getFrequencyData(std::vector<float>& frequencies, std::vector<float>& magnitudes) const;
    
    // Set/get the LUFS value for this stem
    void setLUFS(float lufs);
    float getLUFS() const;
    
    // Set/get the RMS value for this stem
    void setRMS(float rms);
    float getRMS() const;
    
    // Set/get the stereo width value for this stem
    void setWidth(float width);
    float getWidth() const;
    
    // Clear all data
    void clear();
    
    // Check if this stem has valid audio data
    bool hasValidAudio() const;
    
    // Check if this stem has valid frequency data
    bool hasValidFrequencyData() const;

private:
    StemType type;
    juce::String name;
    juce::AudioBuffer<float> audioBuffer;
    std::vector<float> frequencies;
    std::vector<float> magnitudes;
    float lufs = -70.0f;
    float rms = 0.0f;
    float width = 0.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StemData)
};

} // namespace ForensEQ
