#pragma once

#include <JuceHeader.h>
#include "StemData.h"

namespace ForensEQ {

/**
 * Class for analyzing frequency content of audio stems
 */
class StemAnalyzer {
public:
    StemAnalyzer();
    ~StemAnalyzer();
    
    // Analyze a stem and update its frequency data
    bool analyzeStem(StemData& stem);
    
    // Compare two stems and return a similarity score (0.0 to 1.0)
    float compareStemFrequencies(const StemData& stem1, const StemData& stem2);
    
    // Set the FFT size for analysis
    void setFFTSize(int size);
    
    // Get the current FFT size
    int getFFTSize() const;
    
    // Set the window type for FFT analysis
    enum class WindowType {
        Rectangular,
        Hanning,
        Hamming,
        Blackman
    };
    void setWindowType(WindowType type);
    
    // Get the current window type
    WindowType getWindowType() const;
    
    // Calculate LUFS for a stem
    float calculateLUFS(const StemData& stem);
    
    // Calculate RMS for a stem
    float calculateRMS(const StemData& stem);
    
    // Calculate stereo width for a stem
    float calculateWidth(const StemData& stem);

private:
    int fftSize = 2048;
    WindowType windowType = WindowType::Hanning;
    
    // Apply window function to FFT data
    void applyWindow(float* data, int size);
    
    // Convert linear magnitude to dB
    float linearToDecibel(float value);
    
    // Convert dB to linear magnitude
    float decibelToLinear(float dB);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StemAnalyzer)
};

} // namespace ForensEQ
