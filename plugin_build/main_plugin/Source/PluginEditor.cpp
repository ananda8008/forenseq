#include "PluginProcessor.h"
#include "PluginEditor.h"

ForensEQEditor::ForensEQEditor(ForensEQAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Create main component
    mainComponent = std::make_unique<ForensEQMainComponent>();
    addAndMakeVisible(mainComponent.get());
    
    // Set editor size to match main component
    setSize(mainComponent->getWidth(), mainComponent->getHeight());
}

void ForensEQEditor::paint(juce::Graphics& g)
{
    // Background is handled by main component
    g.fillAll(juce::Colours::transparentBlack);
}

void ForensEQEditor::resized()
{
    // Main component fills the entire editor
    mainComponent->setBounds(getLocalBounds());
}
