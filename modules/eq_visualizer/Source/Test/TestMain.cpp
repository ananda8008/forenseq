#include <JuceHeader.h>
#include "../EQVisualizerComponent.h"
#include "../EQVisualizerControls.h"

//==============================================================================
class MainTestComponent : public juce::Component
{
public:
    MainTestComponent()
    {
        // Set up the EQ visualizer
        addAndMakeVisible(eqVisualizer);
        
        // Set up the controls
        addAndMakeVisible(eqControls);
        
        // Set some initial EQ data
        generateDemoEQData();
        
        // Set size
        setSize(900, 700);
        
        // Set title
        setName("ForensEQ EQ Visualizer Test");
        
        // Add status label
        statusLabel.setText("ForensEQ EQ Visualizer Test Application", juce::dontSendNotification);
        statusLabel.setJustificationType(juce::Justification::centred);
        statusLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        statusLabel.setFont(juce::Font(16.0f, juce::Font::bold));
        addAndMakeVisible(statusLabel);
    }

    ~MainTestComponent() override
    {
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(30, 30, 30));
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced(10);
        
        // Position the status label at the top
        statusLabel.setBounds(bounds.removeFromTop(30));
        
        // Position the EQ visualizer in the top part
        eqVisualizer.setBounds(bounds.removeFromTop(bounds.getHeight() * 0.7));
        
        // Add some spacing
        bounds.removeFromTop(10);
        
        // Position the controls in the bottom part
        eqControls.setBounds(bounds);
    }

private:
    ForensEQ::EQVisualizerComponent eqVisualizer;
    ForensEQ::EQVisualizerControls eqControls{eqVisualizer};
    juce::Label statusLabel;
    
    void generateDemoEQData()
    {
        // Create frequency array (logarithmic scale from 20Hz to 20kHz)
        const int numPoints = 100;
        std::vector<float> frequencies(numPoints);
        
        for (int i = 0; i < numPoints; ++i)
        {
            float t = static_cast<float>(i) / static_cast<float>(numPoints - 1);
            frequencies[i] = 20.0f * std::pow(1000.0f, t);
        }
        
        // Create initial magnitude arrays
        std::vector<float> userMagnitudes(numPoints);
        std::vector<float> referenceMagnitudes(numPoints);
        
        // Set initial EQ curves
        for (size_t i = 0; i < frequencies.size(); ++i)
        {
            float freq = frequencies[i];
            
            // User EQ curve - slightly boosted mids
            if (freq < 250.0f)
                userMagnitudes[i] = 0.0f;
            else if (freq < 4000.0f)
                userMagnitudes[i] = 3.0f;
            else
                userMagnitudes[i] = 1.5f;
                
            // Reference EQ curve - slightly boosted bass and highs
            if (freq < 250.0f)
                referenceMagnitudes[i] = 2.0f;
            else if (freq < 4000.0f)
                referenceMagnitudes[i] = 0.0f;
            else
                referenceMagnitudes[i] = 3.0f;
                
            // Smooth transitions
            if (freq > 200.0f && freq < 300.0f)
            {
                float t = (freq - 200.0f) / 100.0f;
                userMagnitudes[i] = 0.0f * (1.0f - t) + 3.0f * t;
                referenceMagnitudes[i] = 2.0f * (1.0f - t) + 0.0f * t;
            }
            else if (freq > 3000.0f && freq < 5000.0f)
            {
                float t = (freq - 3000.0f) / 2000.0f;
                userMagnitudes[i] = 3.0f * (1.0f - t) + 1.5f * t;
                referenceMagnitudes[i] = 0.0f * (1.0f - t) + 3.0f * t;
            }
        }
        
        // Set the data to the visualizer
        eqVisualizer.setUserEQData(frequencies, userMagnitudes);
        eqVisualizer.setReferenceEQData(frequencies, referenceMagnitudes);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainTestComponent)
};

//==============================================================================
class ForensEQTestApplication : public juce::JUCEApplication
{
public:
    ForensEQTestApplication() {}

    const juce::String getApplicationName() override       { return "ForensEQ EQ Visualizer Test"; }
    const juce::String getApplicationVersion() override    { return "1.0.0"; }

    void initialise(const juce::String&) override
    {
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name,
                            juce::Colours::darkgrey,
                            DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainTestComponent(), true);
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
START_JUCE_APPLICATION(ForensEQTestApplication)
