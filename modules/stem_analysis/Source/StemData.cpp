#include "StemData.h"

namespace ForensEQ {

StemData::StemData(StemType type)
    : type(type)
{
    // Set the name based on the stem type
    switch (type) {
        case StemType::Kick:
            name = "Kick";
            break;
        case StemType::Snare:
            name = "Snare";
            break;
        case StemType::Bass:
            name = "Bass";
            break;
        case StemType::Vocals:
            name = "Vocals";
            break;
        case StemType::Other:
            name = "Other";
            break;
        case StemType::Full:
        default:
            name = "Full Mix";
            break;
    }
}

StemData::~StemData()
{
}

juce::String StemData::getName() const
{
    return name;
}

StemType StemData::getType() const
{
    return type;
}

void StemData::setAudioBuffer(const juce::AudioBuffer<float>& buffer)
{
    audioBuffer = buffer;
}

const juce::AudioBuffer<float>& StemData::getAudioBuffer() const
{
    return audioBuffer;
}

void StemData::setFrequencyData(const std::vector<float>& freqs, const std::vector<float>& mags)
{
    frequencies = freqs;
    magnitudes = mags;
}

void StemData::getFrequencyData(std::vector<float>& freqs, std::vector<float>& mags) const
{
    freqs = frequencies;
    mags = magnitudes;
}

void StemData::setLUFS(float value)
{
    lufs = value;
}

float StemData::getLUFS() const
{
    return lufs;
}

void StemData::setRMS(float value)
{
    rms = value;
}

float StemData::getRMS() const
{
    return rms;
}

void StemData::setWidth(float value)
{
    width = value;
}

float StemData::getWidth() const
{
    return width;
}

void StemData::clear()
{
    audioBuffer.clear();
    frequencies.clear();
    magnitudes.clear();
    lufs = -70.0f;
    rms = 0.0f;
    width = 0.0f;
}

bool StemData::hasValidAudio() const
{
    return audioBuffer.getNumSamples() > 0;
}

bool StemData::hasValidFrequencyData() const
{
    return !frequencies.empty() && !magnitudes.empty() && 
           frequencies.size() == magnitudes.size();
}

} // namespace ForensEQ
