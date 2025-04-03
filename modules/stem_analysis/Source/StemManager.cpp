#include "StemManager.h"

namespace ForensEQ {

StemManager::StemManager()
{
    // Create the best available stem isolator
    stemIsolator = StemIsolatorFactory::createBestAvailableIsolator();
}

StemManager::~StemManager()
{
    clearStems();
}

bool StemManager::loadReferenceTrack(const juce::File& audioFile)
{
    // Store the reference track file
    referenceTrackFile = audioFile;
    
    // Clear existing stems
    clearStems();
    
    // Create a full mix stem
    auto fullMixStem = std::make_unique<StemData>(StemType::Full);
    
    // Load the audio file
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(audioFile));
    
    if (reader != nullptr)
    {
        // Create an audio buffer with the same specs as the source file
        juce::AudioBuffer<float> buffer(reader->numChannels, static_cast<int>(reader->lengthInSamples));
        
        // Read the entire file into the buffer
        reader->read(&buffer, 0, buffer.getNumSamples(), 0, true, true);
        
        // Set the buffer to the full mix stem
        fullMixStem->setAudioBuffer(buffer);
        
        // Analyze the full mix stem
        stemAnalyzer.analyzeStem(*fullMixStem);
        
        // Add the full mix stem to the map
        stems[StemType::Full] = std::move(fullMixStem);
        
        // Process stem isolation if we have a valid isolator
        if (stemIsolator != nullptr && stemIsolator->isAvailable())
        {
            bool isolationSuccess = stemIsolator->processStemIsolation(audioFile, stems);
            
            // Analyze each isolated stem
            for (auto& pair : stems)
            {
                if (pair.first != StemType::Full && pair.second != nullptr)
                {
                    stemAnalyzer.analyzeStem(*pair.second);
                }
            }
            
            // Notify listeners that stems have changed
            sendChangeMessage();
            
            return isolationSuccess;
        }
        
        // Notify listeners that stems have changed
        sendChangeMessage();
        
        return true;
    }
    
    return false;
}

juce::File StemManager::getReferenceTrackFile() const
{
    return referenceTrackFile;
}

StemData* StemManager::getStem(StemType type)
{
    auto it = stems.find(type);
    if (it != stems.end())
    {
        return it->second.get();
    }
    return nullptr;
}

StemType StemManager::getActiveStemType() const
{
    return activeStemType;
}

void StemManager::setActiveStemType(StemType type)
{
    if (isStemAvailable(type) && activeStemType != type)
    {
        activeStemType = type;
        sendChangeMessage();
    }
}

std::vector<StemType> StemManager::getAvailableStemTypes() const
{
    std::vector<StemType> types;
    
    for (const auto& pair : stems)
    {
        types.push_back(pair.first);
    }
    
    return types;
}

bool StemManager::isStemAvailable(StemType type) const
{
    return stems.find(type) != stems.end();
}

StemIsolator* StemManager::getStemIsolator()
{
    return stemIsolator.get();
}

StemAnalyzer* StemManager::getStemAnalyzer()
{
    return &stemAnalyzer;
}

float StemManager::compareWithReference(StemType referenceType)
{
    StemData* activeStem = getStem(activeStemType);
    StemData* referenceStem = getStem(referenceType);
    
    if (activeStem != nullptr && referenceStem != nullptr)
    {
        return stemAnalyzer.compareStemFrequencies(*activeStem, *referenceStem);
    }
    
    return 0.0f;
}

int StemManager::getNumLoadedStems() const
{
    return static_cast<int>(stems.size());
}

void StemManager::clearStems()
{
    stems.clear();
    activeStemType = StemType::Full;
    sendChangeMessage();
}

} // namespace ForensEQ
