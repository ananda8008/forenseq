# ForensEQ - Waveform Viewer Module

## Overview

The Waveform Viewer module is the second core component of the ForensEQ plugin, a reference mix analysis tool for audio production. This module provides functionality for loading, visualizing, and interacting with reference audio tracks, designed to work alongside the EQ visualizer module.

## Features

- **Reference Track Loading**: Load up to three reference tracks via drag-and-drop or file selection dialog
- **Waveform Visualization**: Display accurate visual waveforms of loaded reference tracks
- **User Interaction**:
  - Click on waveform to seek to a position
  - Drag across waveform to select a region
  - Switch between reference tracks with buttons
- **Level Matching**: Interface for matching levels between reference and user tracks
- **Time Alignment**: Visual markers for aligning reference tracks with the mix
- **Dark UI Aesthetic**: Consistent with the EQ visualizer module
  - Dark background (charcoal/black)
  - Light text (white/light gray)
  - Modern sans-serif font
  - Clean, minimal, and professional visual style

## Module Structure

```
modules/waveform_viewer/
├── CMakeLists.txt              # Build configuration
├── JuceLibraryCode/            # JUCE library code
├── README.md                   # This documentation file
├── Resources/                  # Resources and assets
└── Source/                     # Source code
    ├── Demo/                   # Demo application
    │   └── Main.cpp            # Demo application entry point
    ├── WaveformViewerComponent.h       # Core waveform viewer component header
    ├── WaveformViewerComponent.cpp     # Core waveform viewer component implementation
    ├── WaveformViewerExtensions.cpp    # Additional functionality
    ├── WaveformDisplay.h               # Waveform visualization component header
    ├── WaveformDisplay.cpp             # Waveform visualization component implementation
    ├── ReferenceTrackLoader.h          # Track loading component header
    ├── ReferenceTrackLoader.cpp        # Track loading component implementation
    ├── TrackInfoComponent.h            # Track information component header
    ├── TrackInfoComponent.cpp          # Track information component implementation
    ├── TrackSwitcherComponent.h        # Track switching component header
    ├── TrackSwitcherComponent.cpp      # Track switching component implementation
    ├── WaveformViewerMainComponent.h   # Main integration component header
    └── WaveformViewerMainComponent.cpp # Main integration component implementation
```

## Integration

The Waveform Viewer module is designed to be easily integrated with other ForensEQ modules:

1. **EQ Visualizer**: Positioned below the EQ visualizer in the UI layout
2. **Stem Isolation**: Will connect with future stem isolation and analysis features
3. **AI Suggestions**: Will integrate with AI-based mix suggestions

## Usage

### Basic Integration

```cpp
// Include the main component
#include "WaveformViewerMainComponent.h"

// Create an instance
ForensEQ::WaveformViewerMainComponent waveformViewer;

// Add to your component hierarchy
addAndMakeVisible(waveformViewer);
```

### Using Individual Components

```cpp
// Include the components you need
#include "WaveformViewerComponent.h"
#include "ReferenceTrackLoader.h"
#include "TrackInfoComponent.h"
#include "TrackSwitcherComponent.h"

// Create instances
ForensEQ::WaveformViewerComponent waveformViewer;
ForensEQ::ReferenceTrackLoader trackLoader(waveformViewer);
ForensEQ::TrackInfoComponent trackInfo(waveformViewer);
ForensEQ::TrackSwitcherComponent trackSwitcher(waveformViewer);

// Add to your component hierarchy
addAndMakeVisible(waveformViewer);
addAndMakeVisible(trackLoader);
addAndMakeVisible(trackInfo);
addAndMakeVisible(trackSwitcher);
```

## Building

The module uses CMake for building:

```bash
cd modules/waveform_viewer
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

1. Actual audio playback functionality
2. More advanced time alignment tools
3. Automatic level matching algorithms
4. Integration with audio analysis tools
5. Support for more audio file formats

## License

This module is part of the ForensEQ plugin and is subject to the same licensing terms as the main project.
