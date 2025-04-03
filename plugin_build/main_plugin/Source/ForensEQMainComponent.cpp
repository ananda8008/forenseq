#include "ForensEQMainComponent.h"

// Forward declarations of module components
// These would typically be included from their respective module headers
namespace WaveformViewer { class WaveformViewerComponent; }
namespace EQVisualizer { class EQVisualizerComponent; }
namespace StemAnalysis { class StemAnalysisComponent; }
namespace LoudnessWidth { class LoudnessWidthComparisonComponent; }
namespace AISuggestions { class SuggestionDrawerComponent; }

ForensEQMainComponent::ForensEQMainComponent()
{
    // Apply theme to this component
    setLookAndFeel(&theme);
    
    // Set up navigation buttons
    setupNavigationButtons();
    
    // Create module components
    createModuleComponents();
    
    // Show default module
    currentModule = "waveform";
    showModule(currentModule);
    
    // Make component visible
    setSize(1200, 800);
    setVisible(true);
}

ForensEQMainComponent::~ForensEQMainComponent()
{
    // Clean up
    setLookAndFeel(nullptr);
    
    // Remove listeners
    waveformViewerButton->removeListener(this);
    eqVisualizerButton->removeListener(this);
    stemAnalysisButton->removeListener(this);
    loudnessWidthButton->removeListener(this);
    aiSuggestionsButton->removeListener(this);
}

void ForensEQMainComponent::paint(juce::Graphics& g)
{
    // Fill background with theme color
    g.fillAll(theme.getBackgroundColor());
    
    // Draw header with subtle gradient
    juce::Rectangle<int> headerArea(0, 0, getWidth(), 60);
    juce::ColourGradient headerGradient(
        theme.getBackgroundAccentColor(),
        0.0f, 0.0f,
        theme.getBackgroundAccentColor().darker(0.2f),
        0.0f, static_cast<float>(headerArea.getHeight()),
        false
    );
    g.setGradientFill(headerGradient);
    g.fillRect(headerArea);
    
    // Draw plugin title with illumination effect
    g.setFont(juce::Font("Helvetica", 28.0f, juce::Font::bold));
    
    // Create illuminated text effect
    juce::Colour textColor = theme.getTextHighlightColor();
    juce::Colour glowColor = theme.getPrimaryColor();
    
    // Draw text glow
    juce::DropShadow glow = theme.getTextGlow(glowColor, 1.2f);
    glow.drawForText(g, "ForensEQ", headerArea.withTrimmedLeft(20).withTrimmedRight(20), 
                    juce::Justification::centredLeft);
    
    // Draw text
    g.setColour(theme.getIlluminatedColor(textColor, 1.2f));
    g.drawText("ForensEQ", headerArea.withTrimmedLeft(20).withTrimmedRight(20), 
              juce::Justification::centredLeft, true);
}

void ForensEQMainComponent::resized()
{
    // Layout navigation buttons in header
    juce::Rectangle<int> headerArea(0, 0, getWidth(), 60);
    
    // Reserve space for title
    int titleWidth = 200;
    int buttonWidth = (getWidth() - titleWidth) / 5;
    int buttonHeight = 40;
    int buttonY = (headerArea.getHeight() - buttonHeight) / 2;
    
    waveformViewerButton->setBounds(titleWidth, buttonY, buttonWidth, buttonHeight);
    eqVisualizerButton->setBounds(titleWidth + buttonWidth, buttonY, buttonWidth, buttonHeight);
    stemAnalysisButton->setBounds(titleWidth + buttonWidth * 2, buttonY, buttonWidth, buttonHeight);
    loudnessWidthButton->setBounds(titleWidth + buttonWidth * 3, buttonY, buttonWidth, buttonHeight);
    aiSuggestionsButton->setBounds(titleWidth + buttonWidth * 4, buttonY, buttonWidth, buttonHeight);
    
    // Layout module components
    juce::Rectangle<int> moduleArea(0, headerArea.getHeight(), getWidth(), getHeight() - headerArea.getHeight());
    
    if (waveformViewerComponent != nullptr)
        waveformViewerComponent->setBounds(moduleArea);
    
    if (eqVisualizerComponent != nullptr)
        eqVisualizerComponent->setBounds(moduleArea);
    
    if (stemAnalysisComponent != nullptr)
        stemAnalysisComponent->setBounds(moduleArea);
    
    if (loudnessWidthComponent != nullptr)
        loudnessWidthComponent->setBounds(moduleArea);
    
    if (aiSuggestionsComponent != nullptr)
        aiSuggestionsComponent->setBounds(moduleArea);
}

void ForensEQMainComponent::buttonClicked(juce::Button* button)
{
    // Handle navigation button clicks
    if (button == waveformViewerButton.get())
        showModule("waveform");
    else if (button == eqVisualizerButton.get())
        showModule("eq");
    else if (button == stemAnalysisButton.get())
        showModule("stem");
    else if (button == loudnessWidthButton.get())
        showModule("loudness");
    else if (button == aiSuggestionsButton.get())
        showModule("ai");
}

void ForensEQMainComponent::showModule(const juce::String& moduleName)
{
    // Hide all components first
    if (waveformViewerComponent != nullptr)
        waveformViewerComponent->setVisible(false);
    
    if (eqVisualizerComponent != nullptr)
        eqVisualizerComponent->setVisible(false);
    
    if (stemAnalysisComponent != nullptr)
        stemAnalysisComponent->setVisible(false);
    
    if (loudnessWidthComponent != nullptr)
        loudnessWidthComponent->setVisible(false);
    
    if (aiSuggestionsComponent != nullptr)
        aiSuggestionsComponent->setVisible(false);
    
    // Reset button states
    waveformViewerButton->setToggleState(false, juce::dontSendNotification);
    eqVisualizerButton->setToggleState(false, juce::dontSendNotification);
    stemAnalysisButton->setToggleState(false, juce::dontSendNotification);
    loudnessWidthButton->setToggleState(false, juce::dontSendNotification);
    aiSuggestionsButton->setToggleState(false, juce::dontSendNotification);
    
    // Show selected module
    if (moduleName == "waveform")
    {
        if (waveformViewerComponent != nullptr)
            waveformViewerComponent->setVisible(true);
        waveformViewerButton->setToggleState(true, juce::dontSendNotification);
    }
    else if (moduleName == "eq")
    {
        if (eqVisualizerComponent != nullptr)
            eqVisualizerComponent->setVisible(true);
        eqVisualizerButton->setToggleState(true, juce::dontSendNotification);
    }
    else if (moduleName == "stem")
    {
        if (stemAnalysisComponent != nullptr)
            stemAnalysisComponent->setVisible(true);
        stemAnalysisButton->setToggleState(true, juce::dontSendNotification);
    }
    else if (moduleName == "loudness")
    {
        if (loudnessWidthComponent != nullptr)
            loudnessWidthComponent->setVisible(true);
        loudnessWidthButton->setToggleState(true, juce::dontSendNotification);
    }
    else if (moduleName == "ai")
    {
        if (aiSuggestionsComponent != nullptr)
            aiSuggestionsComponent->setVisible(true);
        aiSuggestionsButton->setToggleState(true, juce::dontSendNotification);
    }
    
    currentModule = moduleName;
    repaint();
}

void ForensEQMainComponent::createModuleComponents()
{
    // Create instances of module components
    // In a real implementation, these would be created from their respective module classes
    
    // For now, create placeholder components that will be replaced with actual module components
    waveformViewerComponent = std::make_unique<juce::Component>();
    eqVisualizerComponent = std::make_unique<juce::Component>();
    stemAnalysisComponent = std::make_unique<juce::Component>();
    loudnessWidthComponent = std::make_unique<juce::Component>();
    aiSuggestionsComponent = std::make_unique<juce::Component>();
    
    // Add components as children
    addAndMakeVisible(waveformViewerComponent.get());
    addAndMakeVisible(eqVisualizerComponent.get());
    addAndMakeVisible(stemAnalysisComponent.get());
    addAndMakeVisible(loudnessWidthComponent.get());
    addAndMakeVisible(aiSuggestionsComponent.get());
    
    // Hide all initially
    waveformViewerComponent->setVisible(false);
    eqVisualizerComponent->setVisible(false);
    stemAnalysisComponent->setVisible(false);
    loudnessWidthComponent->setVisible(false);
    aiSuggestionsComponent->setVisible(false);
    
    // Apply theme to all components
    applyThemeToAllComponents();
}

void ForensEQMainComponent::setupNavigationButtons()
{
    // Create navigation buttons
    waveformViewerButton = std::make_unique<juce::TextButton>("Waveform");
    eqVisualizerButton = std::make_unique<juce::TextButton>("EQ");
    stemAnalysisButton = std::make_unique<juce::TextButton>("Stems");
    loudnessWidthButton = std::make_unique<juce::TextButton>("Loudness/Width");
    aiSuggestionsButton = std::make_unique<juce::TextButton>("AI Suggestions");
    
    // Configure buttons
    waveformViewerButton->setClickingTogglesState(true);
    eqVisualizerButton->setClickingTogglesState(true);
    stemAnalysisButton->setClickingTogglesState(true);
    loudnessWidthButton->setClickingTogglesState(true);
    aiSuggestionsButton->setClickingTogglesState(true);
    
    // Add listeners
    waveformViewerButton->addListener(this);
    eqVisualizerButton->addListener(this);
    stemAnalysisButton->addListener(this);
    loudnessWidthButton->addListener(this);
    aiSuggestionsButton->addListener(this);
    
    // Add buttons as children
    addAndMakeVisible(waveformViewerButton.get());
    addAndMakeVisible(eqVisualizerButton.get());
    addAndMakeVisible(stemAnalysisButton.get());
    addAndMakeVisible(loudnessWidthButton.get());
    addAndMakeVisible(aiSuggestionsButton.get());
}

void ForensEQMainComponent::applyThemeToAllComponents()
{
    // Apply theme to all child components
    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        juce::Component* child = getChildComponent(i);
        if (child != nullptr)
            child->setLookAndFeel(&theme);
    }
}
