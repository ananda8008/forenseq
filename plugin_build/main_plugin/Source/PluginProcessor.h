#pragma once

#include <JuceHeader.h>

/**
 * ForensEQAudioProcessor - Main audio processor for the ForensEQ plugin
 * 
 * Handles audio processing and parameter management for all integrated modules
 */
class ForensEQAudioProcessor : public juce::AudioProcessor
{
public:
    ForensEQAudioProcessor();
    ~ForensEQAudioProcessor() override;

    // AudioProcessor methods
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    // Editor methods
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    // Program and state methods
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

private:
    // Audio analysis data
    juce::AudioBuffer<float> analysisBuffer;
    
    // Parameters
    juce::AudioProcessorValueTreeState parameters;
    
    // Create parameters
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ForensEQAudioProcessor)
};
