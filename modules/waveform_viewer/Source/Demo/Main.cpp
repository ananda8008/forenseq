#include <JuceHeader.h>
#include "../WaveformViewerMainComponent.h"

//==============================================================================
class MainComponent : public juce::Component
{
public:
    MainComponent()
    {
        // Add the main waveform viewer component
        addAndMakeVisible(waveformViewerMain);
        
        // Set size
        setSize(900, 700);
    }

    ~MainComponent() override
    {
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(20, 20, 20));
    }

    void resized() override
    {
        // Position the main component to fill the entire window
        waveformViewerMain.setBounds(getLocalBounds().reduced(10));
    }

private:
    ForensEQ::WaveformViewerMainComponent waveformViewerMain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

//==============================================================================
class ForensEQDemoApplication : public juce::JUCEApplication
{
public:
    ForensEQDemoApplication() {}

    const juce::String getApplicationName() override       { return "ForensEQ Waveform Viewer Demo"; }
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
            setContentOwned(new MainComponent(), true);
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
START_JUCE_APPLICATION(ForensEQDemoApplication)
