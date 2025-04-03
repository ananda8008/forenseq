#pragma once

#include <JuceHeader.h>
#include "StemData.h"

namespace ForensEQ {

/**
 * Interface for stem isolation systems
 */
class StemIsolator {
public:
    StemIsolator() = default;
    virtual ~StemIsolator() = default;
    
    // Check if the isolator is available on this system
    virtual bool isAvailable() const = 0;
    
    // Process an audio file and isolate stems
    virtual bool processStemIsolation(const juce::File& audioFile, 
                                     std::map<StemType, std::unique_ptr<StemData>>& stems) = 0;
    
    // Get the name of this isolator
    virtual juce::String getName() const = 0;
    
    // Get a description of this isolator
    virtual juce::String getDescription() const = 0;
    
    // Get the supported stem types
    virtual std::vector<StemType> getSupportedStemTypes() const = 0;
};

/**
 * Factory for creating stem isolators
 */
class StemIsolatorFactory {
public:
    static std::vector<std::unique_ptr<StemIsolator>> createAvailableIsolators();
    static std::unique_ptr<StemIsolator> createBestAvailableIsolator();
};

/**
 * Logic Pro stem isolator implementation
 */
class LogicProStemIsolator : public StemIsolator {
public:
    LogicProStemIsolator();
    ~LogicProStemIsolator() override;
    
    bool isAvailable() const override;
    bool processStemIsolation(const juce::File& audioFile, 
                             std::map<StemType, std::unique_ptr<StemData>>& stems) override;
    juce::String getName() const override;
    juce::String getDescription() const override;
    std::vector<StemType> getSupportedStemTypes() const override;
    
private:
    bool checkLogicProAvailability() const;
};

/**
 * Demucs stem isolator implementation (open-source alternative)
 */
class DemucsStemIsolator : public StemIsolator {
public:
    DemucsStemIsolator();
    ~DemucsStemIsolator() override;
    
    bool isAvailable() const override;
    bool processStemIsolation(const juce::File& audioFile, 
                             std::map<StemType, std::unique_ptr<StemData>>& stems) override;
    juce::String getName() const override;
    juce::String getDescription() const override;
    std::vector<StemType> getSupportedStemTypes() const override;
    
private:
    bool checkDemucsAvailability() const;
    juce::File getDemucsExecutable() const;
    bool runDemucsProcess(const juce::File& inputFile, const juce::File& outputDir);
    bool loadProcessedStems(const juce::File& outputDir, 
                           std::map<StemType, std::unique_ptr<StemData>>& stems);
};

/**
 * Mock stem isolator for testing and development
 */
class MockStemIsolator : public StemIsolator {
public:
    MockStemIsolator();
    ~MockStemIsolator() override;
    
    bool isAvailable() const override;
    bool processStemIsolation(const juce::File& audioFile, 
                             std::map<StemType, std::unique_ptr<StemData>>& stems) override;
    juce::String getName() const override;
    juce::String getDescription() const override;
    std::vector<StemType> getSupportedStemTypes() const override;
    
private:
    void generateMockStem(StemType type, const juce::File& sourceFile, 
                         std::map<StemType, std::unique_ptr<StemData>>& stems);
};

} // namespace ForensEQ
