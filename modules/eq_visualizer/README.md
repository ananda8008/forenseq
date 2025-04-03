# ForensEQ - EQ Visualizer Module

## Overview

The EQ Visualizer module is the first core component of the ForensEQ plugin, a reference mix analysis tool for audio production. This module provides a real-time dual EQ visualization that allows users to compare their mix with a reference track, highlighting areas of similarity and difference.

## Features

- **Dual EQ Visualization**: Displays two EQ curves simultaneously - one for the user's mix and one for a reference track
- **Distinct Color Coding**: User mix (blue) and reference track (orange) are clearly differentiated
- **Overlap Highlighting**: Areas where the curves align closely are highlighted with a configurable gold shimmer/glow effect
- **Particle Animation**: Visual particle effects and sparkles animate along the curves for enhanced visual feedback
- **Curve Toggling**: Users can toggle which curve appears in the foreground
- **Dark Theme**: Modern dark-themed interface with light text for optimal readability
- **Frequency Band Labels**: Clear markers for Sub, Low, Mid, High-Mid, and High frequency bands
- **Configurable Visual Effects**: Adjustable particle rate and glow intensity

## Module Structure

```
modules/eq_visualizer/
├── CMakeLists.txt              # Build configuration
├── JuceLibraryCode/            # JUCE library code
├── README.md                   # This documentation file
├── Resources/                  # Resources and assets
└── Source/                     # Source code
    ├── Demo/                   # Demo application
    │   └── Main.cpp            # Demo application entry point
    ├── Test/                   # Test application
    │   └── TestMain.cpp        # Test application entry point
    ├── EQVisualizerComponent.h # Core visualization component header
    ├── EQVisualizerComponent.cpp # Core visualization component implementation
    ├── EQVisualizerControls.h  # UI controls header
    ├── EQVisualizerControls.cpp # UI controls implementation
    └── EQVisualizerExtensions.cpp # Additional functionality
```

## Integration

The EQ Visualizer module is designed to be easily integrated with other ForensEQ modules:

1. **Stem Analysis**: Future integration will allow visualization of individual stems
2. **AI Suggestions**: Will connect with AI analysis to suggest EQ adjustments
3. **Width/Loudness Modules**: Will complement width and loudness visualization

## Usage

### Basic Integration

```cpp
// Include the component
#include "EQVisualizerComponent.h"

// Create an instance
ForensEQ::EQVisualizerComponent eqVisualizer;

// Add to your component hierarchy
addAndMakeVisible(eqVisualizer);

// Set EQ data
std::vector<float> frequencies = { /* frequency points */ };
std::vector<float> magnitudes = { /* magnitude values in dB */ };
eqVisualizer.setUserEQData(frequencies, magnitudes);
eqVisualizer.setReferenceEQData(referenceFrequencies, referenceMagnitudes);
```

### With Controls

```cpp
// Include the components
#include "EQVisualizerComponent.h"
#include "EQVisualizerControls.h"

// Create instances
ForensEQ::EQVisualizerComponent eqVisualizer;
ForensEQ::EQVisualizerControls eqControls(eqVisualizer);

// Add to your component hierarchy
addAndMakeVisible(eqVisualizer);
addAndMakeVisible(eqControls);
```

## Building

The module uses CMake for building:

```bash
cd modules/eq_visualizer
mkdir build
cd build
cmake ..
cmake --build .
```

## Dependencies

- JUCE Framework 7.0.5 or later
- C++17 compatible compiler
- CMake 3.15 or later

## Future Development

This module is designed to be extended with:

1. Additional analysis features for more detailed frequency comparison
2. Integration with audio processing for real-time analysis
3. Preset management for saving and loading reference curves
4. Export functionality for sharing analysis results

## License

This module is part of the ForensEQ plugin and is subject to the same licensing terms as the main project.
