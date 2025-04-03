#include "StemAnalyzer.h"

namespace ForensEQ {

StemAnalyzer::StemAnalyzer()
{
}

StemAnalyzer::~StemAnalyzer()
{
}

bool StemAnalyzer::analyzeStem(StemData& stem)
{
    if (!stem.hasValidAudio())
        return false;
    
    const auto& audioBuffer = stem.getAudioBuffer();
    
    // Prepare FFT data
    juce::dsp::FFT fft(static_cast<int>(std::log2(fftSize)));
    
    // Create window and FFT buffers
    std::vector<float> windowBuffer(fftSize, 0.0f);
    std::vector<float> fftBuffer(fftSize * 2, 0.0f); // Complex data (real/imag pairs)
    
    // Number of FFT frames to analyze
    int numSamples = audioBuffer.getNumSamples();
    int numFrames = numSamples / (fftSize / 2) - 1;
    numFrames = juce::jmax(1, numFrames);
    
    // Frequency resolution
    float sampleRate = 44100.0f; // Assuming 44.1kHz, would be retrieved from actual audio
    float freqResolution = sampleRate / fftSize;
    
    // Prepare frequency and magnitude vectors
    std::vector<float> frequencies;
    std::vector<float> magnitudes;
    
    // We're only interested in the first half of the FFT output (up to Nyquist frequency)
    for (int i = 0; i < fftSize / 2; ++i)
    {
        frequencies.push_back(i * freqResolution);
        magnitudes.push_back(0.0f);
    }
    
    // Process each frame
    for (int frame = 0; frame < numFrames; ++frame)
    {
        // Get the start sample for this frame (with 50% overlap)
        int startSample = frame * (fftSize / 2);
        
        // Clear the FFT buffer
        std::fill(fftBuffer.begin(), fftBuffer.end(), 0.0f);
        
        // Copy audio data to the FFT buffer (averaging channels)
        for (int channel = 0; channel < audioBuffer.getNumChannels(); ++channel)
        {
            const float* channelData = audioBuffer.getReadPointer(channel);
            
            for (int i = 0; i < fftSize; ++i)
            {
                if (startSample + i < numSamples)
                {
                    // Add to real part (even indices)
                    fftBuffer[i * 2] += channelData[startSample + i] / audioBuffer.getNumChannels();
                }
            }
        }
        
        // Apply window function
        applyWindow(fftBuffer.data(), fftSize);
        
        // Perform FFT
        fft.performRealOnlyForwardTransform(fftBuffer.data(), true);
        
        // Calculate magnitudes and accumulate
        for (int i = 0; i < fftSize / 2; ++i)
        {
            float real = fftBuffer[i * 2];
            float imag = fftBuffer[i * 2 + 1];
            float magnitude = std::sqrt(real * real + imag * imag);
            
            // Convert to dB and accumulate
            magnitudes[i] += linearToDecibel(magnitude);
        }
    }
    
    // Average the magnitudes
    for (auto& mag : magnitudes)
    {
        mag /= numFrames;
    }
    
    // Set the frequency data to the stem
    stem.setFrequencyData(frequencies, magnitudes);
    
    // Calculate and set LUFS, RMS, and width
    stem.setLUFS(calculateLUFS(stem));
    stem.setRMS(calculateRMS(stem));
    stem.setWidth(calculateWidth(stem));
    
    return true;
}

float StemAnalyzer::compareStemFrequencies(const StemData& stem1, const StemData& stem2)
{
    if (!stem1.hasValidFrequencyData() || !stem2.hasValidFrequencyData())
        return 0.0f;
    
    std::vector<float> freq1, mag1, freq2, mag2;
    stem1.getFrequencyData(freq1, mag1);
    stem2.getFrequencyData(freq2, mag2);
    
    // Ensure we have data to compare
    if (freq1.empty() || mag1.empty() || freq2.empty() || mag2.empty())
        return 0.0f;
    
    // Calculate similarity score using correlation
    float sumProduct = 0.0f;
    float sumSquares1 = 0.0f;
    float sumSquares2 = 0.0f;
    
    // We need to interpolate if the frequency points don't match
    size_t index1 = 0;
    size_t index2 = 0;
    
    while (index1 < freq1.size() && index2 < freq2.size())
    {
        float f1 = freq1[index1];
        float f2 = freq2[index2];
        
        float m1, m2;
        
        if (std::abs(f1 - f2) < 0.01f)
        {
            // Frequencies match
            m1 = mag1[index1];
            m2 = mag2[index2];
            index1++;
            index2++;
        }
        else if (f1 < f2)
        {
            // Interpolate m2
            m1 = mag1[index1];
            
            if (index2 > 0)
            {
                float f2Prev = freq2[index2 - 1];
                float m2Prev = mag2[index2 - 1];
                float t = (f1 - f2Prev) / (f2 - f2Prev);
                m2 = m2Prev + t * (mag2[index2] - m2Prev);
            }
            else
            {
                m2 = mag2[index2];
            }
            
            index1++;
        }
        else
        {
            // Interpolate m1
            m2 = mag2[index2];
            
            if (index1 > 0)
            {
                float f1Prev = freq1[index1 - 1];
                float m1Prev = mag1[index1 - 1];
                float t = (f2 - f1Prev) / (f1 - f1Prev);
                m1 = m1Prev + t * (mag1[index1] - m1Prev);
            }
            else
            {
                m1 = mag1[index1];
            }
            
            index2++;
        }
        
        sumProduct += m1 * m2;
        sumSquares1 += m1 * m1;
        sumSquares2 += m2 * m2;
    }
    
    // Calculate correlation coefficient
    float correlation = 0.0f;
    if (sumSquares1 > 0.0f && sumSquares2 > 0.0f)
    {
        correlation = sumProduct / (std::sqrt(sumSquares1) * std::sqrt(sumSquares2));
    }
    
    // Convert to similarity score (0.0 to 1.0)
    return (correlation + 1.0f) / 2.0f;
}

void StemAnalyzer::setFFTSize(int size)
{
    // Ensure FFT size is a power of 2
    fftSize = 1 << static_cast<int>(std::log2(size) + 0.5f);
}

int StemAnalyzer::getFFTSize() const
{
    return fftSize;
}

void StemAnalyzer::setWindowType(WindowType type)
{
    windowType = type;
}

StemAnalyzer::WindowType StemAnalyzer::getWindowType() const
{
    return windowType;
}

float StemAnalyzer::calculateLUFS(const StemData& stem)
{
    if (!stem.hasValidAudio())
        return -70.0f;
    
    const auto& audioBuffer = stem.getAudioBuffer();
    
    // This is a simplified LUFS calculation
    // A real implementation would follow the ITU-R BS.1770 standard
    
    float sum = 0.0f;
    int numSamples = audioBuffer.getNumSamples();
    
    for (int channel = 0; channel < audioBuffer.getNumChannels(); ++channel)
    {
        const float* data = audioBuffer.getReadPointer(channel);
        
        for (int i = 0; i < numSamples; ++i)
        {
            float sample = data[i];
            sum += sample * sample;
        }
    }
    
    float rms = std::sqrt(sum / (numSamples * audioBuffer.getNumChannels()));
    
    // Convert to LUFS (very approximate)
    return linearToDecibel(rms) - 10.0f;
}

float StemAnalyzer::calculateRMS(const StemData& stem)
{
    if (!stem.hasValidAudio())
        return 0.0f;
    
    const auto& audioBuffer = stem.getAudioBuffer();
    
    float sum = 0.0f;
    int numSamples = audioBuffer.getNumSamples();
    
    for (int channel = 0; channel < audioBuffer.getNumChannels(); ++channel)
    {
        const float* data = audioBuffer.getReadPointer(channel);
        
        for (int i = 0; i < numSamples; ++i)
        {
            float sample = data[i];
            sum += sample * sample;
        }
    }
    
    return std::sqrt(sum / (numSamples * audioBuffer.getNumChannels()));
}

float StemAnalyzer::calculateWidth(const StemData& stem)
{
    if (!stem.hasValidAudio())
        return 0.0f;
    
    const auto& audioBuffer = stem.getAudioBuffer();
    
    // Need at least stereo for width calculation
    if (audioBuffer.getNumChannels() < 2)
        return 0.0f;
    
    float sumCorrelation = 0.0f;
    float sumLeft = 0.0f;
    float sumRight = 0.0f;
    int numSamples = audioBuffer.getNumSamples();
    
    const float* leftData = audioBuffer.getReadPointer(0);
    const float* rightData = audioBuffer.getReadPointer(1);
    
    for (int i = 0; i < numSamples; ++i)
    {
        float left = leftData[i];
        float right = rightData[i];
        
        sumCorrelation += left * right;
        sumLeft += left * left;
        sumRight += right * right;
    }
    
    // Calculate correlation coefficient
    float correlation = 0.0f;
    if (sumLeft > 0.0f && sumRight > 0.0f)
    {
        correlation = sumCorrelation / (std::sqrt(sumLeft) * std::sqrt(sumRight));
    }
    
    // Convert correlation to width (1.0 = fully correlated/mono, 0.0 = uncorrelated)
    // Invert so that 1.0 = wide, 0.0 = mono
    return 1.0f - std::abs(correlation);
}

void StemAnalyzer::applyWindow(float* data, int size)
{
    switch (windowType)
    {
        case WindowType::Rectangular:
            // No windowing
            break;
            
        case WindowType::Hanning:
            for (int i = 0; i < size; ++i)
            {
                float factor = 0.5f * (1.0f - std::cos(2.0f * juce::MathConstants<float>::pi * i / (size - 1)));
                data[i * 2] *= factor;
            }
            break;
            
        case WindowType::Hamming:
            for (int i = 0; i < size; ++i)
            {
                float factor = 0.54f - 0.46f * std::cos(2.0f * juce::MathConstants<float>::pi * i / (size - 1));
                data[i * 2] *= factor;
            }
            break;
            
        case WindowType::Blackman:
            for (int i = 0; i < size; ++i)
            {
                float x = 2.0f * juce::MathConstants<float>::pi * i / (size - 1);
                float factor = 0.42f - 0.5f * std::cos(x) + 0.08f * std::cos(2.0f * x);
                data[i * 2] *= factor;
            }
            break;
    }
}

float StemAnalyzer::linearToDecibel(float value)
{
    return 20.0f * std::log10(value + 1.0e-6f);
}

float StemAnalyzer::decibelToLinear(float dB)
{
    return std::pow(10.0f, dB / 20.0f);
}

} // namespace ForensEQ
