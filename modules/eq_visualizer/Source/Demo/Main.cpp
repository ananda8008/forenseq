#include <JuceHeader.h>
#include "../EQVisualizerComponent.h"

//==============================================================================
class MainComponent : public juce::Component
{
public:
    MainComponent()
    {
        // Set up the EQ visualizer
        addAndMakeVisible(eqVisualizer);
        
        // Create toggle button for switching curve order
        toggleButton.setButtonText("Toggle Curve Order");
        toggleButton.onClick = [this] { toggleCurveOrder(); };
        addAndMakeVisible(toggleButton);
        
        // Create sliders for manipulating EQ curves
        setupSliders();
        
        // Set some initial EQ data
        generateDemoEQData();
        
        // Set size
        setSize(800, 600);
    }

    ~MainComponent() override
    {
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(30, 30, 30));
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced(10);
        
        // Position the toggle button at the top
        toggleButton.setBounds(bounds.removeFromTop(30).reduced(5, 0));
        
        // Position the sliders at the bottom
        auto sliderArea = bounds.removeFromBottom(120);
        
        userBassSlider.setBounds(sliderArea.removeFromLeft(120).reduced(5));
        userMidSlider.setBounds(sliderArea.removeFromLeft(120).reduced(5));
        userTrebleSlider.setBounds(sliderArea.removeFromLeft(120).reduced(5));
        
        refBassSlider.setBounds(sliderArea.removeFromLeft(120).reduced(5));
        refMidSlider.setBounds(sliderArea.removeFromLeft(120).reduced(5));
        refTrebleSlider.setBounds(sliderArea.removeFromLeft(120).reduced(5));
        
        // Position the EQ visualizer in the remaining space
        eqVisualizer.setBounds(bounds);
    }

private:
    ForensEQ::EQVisualizerComponent eqVisualizer;
    juce::TextButton toggleButton;
    
    // Sliders for manipulating EQ curves
    juce::Slider userBassSlider, userMidSlider, userTrebleSlider;
    juce::Slider refBassSlider, refMidSlider, refTrebleSlider;
    
    // Labels for sliders
    juce::Label userBassLabel, userMidLabel, userTrebleLabel;
    juce::Label refBassLabel, refMidLabel, refTrebleLabel;
    
    void setupSliders()
    {
        // User EQ sliders
        setupSlider(userBassSlider, userBassLabel, "User Bass", -12.0, 12.0, 0.0);
        setupSlider(userMidSlider, userMidLabel, "User Mid", -12.0, 12.0, 0.0);
        setupSlider(userTrebleSlider, userTrebleLabel, "User Treble", -12.0, 12.0, 0.0);
        
        // Reference EQ sliders
        setupSlider(refBassSlider, refBassLabel, "Ref Bass", -12.0, 12.0, 0.0);
        setupSlider(refMidSlider, refMidLabel, "Ref Mid", -12.0, 12.0, 0.0);
        setupSlider(refTrebleSlider, refTrebleLabel, "Ref Treble", -12.0, 12.0, 0.0);
        
        // Add listeners
        userBassSlider.onValueChange = [this] { updateUserEQCurve(); };
        userMidSlider.onValueChange = [this] { updateUserEQCurve(); };
        userTrebleSlider.onValueChange = [this] { updateUserEQCurve(); };
        
        refBassSlider.onValueChange = [this] { updateReferenceEQCurve(); };
        refMidSlider.onValueChange = [this] { updateReferenceEQCurve(); };
        refTrebleSlider.onValueChange = [this] { updateReferenceEQCurve(); };
    }
    
    void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& text, 
                    double min, double max, double initial)
    {
        slider.setRange(min, max);
        slider.setValue(initial);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
        slider.setSliderStyle(juce::Slider::LinearVertical);
        slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        
        label.setText(text, juce::dontSendNotification);
        label.attachToComponent(&slider, false);
        label.setJustificationType(juce::Justification::centred);
        label.setColour(juce::Label::textColourId, juce::Colours::white);
        
        addAndMakeVisible(slider);
        addAndMakeVisible(label);
    }
    
    void toggleCurveOrder()
    {
        eqVisualizer.toggleCurveOrder();
        toggleButton.setButtonText(eqVisualizer.isUserCurveOnTop() ? 
                                 "User Curve on Top (Click to Toggle)" : 
                                 "Reference Curve on Top (Click to Toggle)");
    }
    
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
        updateEQCurves(frequencies, userMagnitudes, referenceMagnitudes);
        
        // Set the data to the visualizer
        eqVisualizer.setUserEQData(frequencies, userMagnitudes);
        eqVisualizer.setReferenceEQData(frequencies, referenceMagnitudes);
    }
    
    void updateUserEQCurve()
    {
        // Create frequency array
        const int numPoints = 100;
        std::vector<float> frequencies(numPoints);
        std::vector<float> magnitudes(numPoints);
        
        for (int i = 0; i < numPoints; ++i)
        {
            float t = static_cast<float>(i) / static_cast<float>(numPoints - 1);
            frequencies[i] = 20.0f * std::pow(1000.0f, t);
            
            // Apply EQ curve based on slider values
            float freq = frequencies[i];
            float bassGain = static_cast<float>(userBassSlider.getValue());
            float midGain = static_cast<float>(userMidSlider.getValue());
            float trebleGain = static_cast<float>(userTrebleSlider.getValue());
            
            // Simple 3-band EQ model
            if (freq < 250.0f)
                magnitudes[i] = bassGain;
            else if (freq < 4000.0f)
                magnitudes[i] = midGain;
            else
                magnitudes[i] = trebleGain;
                
            // Smooth transitions between bands
            if (freq > 200.0f && freq < 300.0f)
            {
                float t = (freq - 200.0f) / 100.0f;
                magnitudes[i] = bassGain * (1.0f - t) + midGain * t;
            }
            else if (freq > 3000.0f && freq < 5000.0f)
            {
                float t = (freq - 3000.0f) / 2000.0f;
                magnitudes[i] = midGain * (1.0f - t) + trebleGain * t;
            }
        }
        
        // Update the visualizer
        eqVisualizer.setUserEQData(frequencies, magnitudes);
    }
    
    void updateReferenceEQCurve()
    {
        // Create frequency array
        const int numPoints = 100;
        std::vector<float> frequencies(numPoints);
        std::vector<float> magnitudes(numPoints);
        
        for (int i = 0; i < numPoints; ++i)
        {
            float t = static_cast<float>(i) / static_cast<float>(numPoints - 1);
            frequencies[i] = 20.0f * std::pow(1000.0f, t);
            
            // Apply EQ curve based on slider values
            float freq = frequencies[i];
            float bassGain = static_cast<float>(refBassSlider.getValue());
            float midGain = static_cast<float>(refMidSlider.getValue());
            float trebleGain = static_cast<float>(refTrebleSlider.getValue());
            
            // Simple 3-band EQ model
            if (freq < 250.0f)
                magnitudes[i] = bassGain;
            else if (freq < 4000.0f)
                magnitudes[i] = midGain;
            else
                magnitudes[i] = trebleGain;
                
            // Smooth transitions between bands
            if (freq > 200.0f && freq < 300.0f)
            {
                float t = (freq - 200.0f) / 100.0f;
                magnitudes[i] = bassGain * (1.0f - t) + midGain * t;
            }
            else if (freq > 3000.0f && freq < 5000.0f)
            {
                float t = (freq - 3000.0f) / 2000.0f;
                magnitudes[i] = midGain * (1.0f - t) + trebleGain * t;
            }
        }
        
        // Update the visualizer
        eqVisualizer.setReferenceEQData(frequencies, magnitudes);
    }
    
    void updateEQCurves(const std::vector<float>& frequencies, 
                       std::vector<float>& userMagnitudes,
                       std::vector<float>& referenceMagnitudes)
    {
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
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

//==============================================================================
class ForensEQDemoApplication : public juce::JUCEApplication
{
public:
    ForensEQDemoApplication() {}

    const juce::String getApplicationName() override       { return "ForensEQ EQ Visualizer Demo"; }
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
