#include "PluginProcessor.h"
#include "ForensEQMainComponent.h"

/**
 * ForensEQEditor - Main editor component for the ForensEQ plugin
 * 
 * Creates and manages the main UI component
 */
class ForensEQEditor : public juce::AudioProcessorEditor
{
public:
    ForensEQEditor(ForensEQAudioProcessor& p)
        : AudioProcessorEditor(&p), processor(p)
    {
        // Create main component
        mainComponent = std::make_unique<ForensEQMainComponent>();
        addAndMakeVisible(mainComponent.get());
        
        // Set editor size to match main component
        setSize(mainComponent->getWidth(), mainComponent->getHeight());
    }
    
    ~ForensEQEditor() override = default;
    
    void paint(juce::Graphics& g) override
    {
        // Background is handled by main component
        g.fillAll(juce::Colours::transparentBlack);
    }
    
    void resized() override
    {
        // Main component fills the entire editor
        mainComponent->setBounds(getLocalBounds());
    }
    
private:
    // Reference to the processor
    ForensEQAudioProcessor& processor;
    
    // Main component
    std::unique_ptr<ForensEQMainComponent> mainComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ForensEQEditor)
};
