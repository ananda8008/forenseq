# ForensEQ - Stem Analysis Module

## Overview

The Stem Analysis module is the third core component of the ForensEQ plugin, a reference mix analysis tool for audio production. This module provides functionality for isolating and analyzing individual stems from audio tracks, and integrates with the previously built EQ visualizer and waveform viewer modules.

## Features

- **Stem Isolation**: Separates audio into individual stems (Kick, Snare, Bass, Vocals, Other)
- **Frequency Analysis**: Analyzes frequency content of each isolated stem
- **Data Integration**: Routes stem EQ data to the visual EQ system
- **Stem Selection UI**: Allows users to toggle between different stem views
- **Comprehensive Data Model**: Stores name, audio range, EQ data, LUFS/RMS, and width for each stem
- **Integration Bridges**: Connects with existing EQ visualizer and waveform viewer modules

## Stem Isolation Method

The module implements a flexible approach to stem isolation with multiple options:

1. **Logic Pro Integration** (Primary Method): When available, the module interfaces with Logic Pro's built-in stem separation technology, which provides high-quality separation with minimal artifacts.

2. **Demucs Fallback** (Alternative Method): For environments without Logic Pro, the module can use the open-source Demucs library, a state-of-the-art music source separation system.

3. **Mock Isolation** (Development/Testing): A built-in mock isolation system is included for development and testing purposes, which simulates stem separation by applying different frequency filters to the original audio.

The system automatically selects the best available method based on the current environment.

## Module Structure

```
modules/stem_analysis/
├── CMakeLists.txt              # Build configuration
├── JuceLibraryCode/            # JUCE library code
├── README.md                   # This documentation file
├── Resources/                  # Resources and assets
└── Source/                     # Source code
    ├── StemData.h              # Stem data model header
    ├── StemData.cpp            # Stem data model implementation
    ├── StemIsolator.h          # Stem isolation interface header
    ├── StemIsolator.cpp        # Stem isolation implementation
    ├── StemAnalyzer.h          # Frequency analysis header
    ├── StemAnalyzer.cpp        # Frequency analysis implementation
    ├── StemManager.h           # Stem management header
    ├── StemManager.cpp         # Stem management implementation
    ├── StemSelectorComponent.h # UI for stem selection header
    ├── StemSelectorComponent.cpp # UI for stem selection implementation
    ├── StemInfoComponent.h     # UI for stem info display header
    ├── StemInfoComponent.cpp   # UI for stem info display implementation
    ├── StemAnalysisComponent.h # Main component header
    ├── StemAnalysisComponent.cpp # Main component implementation
    ├── StemToEQBridge.h        # EQ integration bridge header
    ├── StemToEQBridge.cpp      # EQ integration bridge implementation
    ├── StemToWaveformBridge.h  # Waveform integration bridge header
    └── StemToWaveformBridge.cpp # Waveform integration bridge implementation
```

## Integration with Other Modules

### EQ Visualizer Integration

The Stem Analysis module connects with the EQ visualizer module through the `StemToEQBridge` class, which:

- Provides frequency data from the selected stem to the EQ visualizer
- Updates the EQ display when the stem selection changes
- Enables comparison between stem EQ curves and reference tracks

### Waveform Viewer Integration

Integration with the waveform viewer module is handled by the `StemToWaveformBridge` class, which:

- Provides audio buffer data from the selected stem to the waveform viewer
- Updates the waveform display when the stem selection changes
- Enables visualization of individual stem waveforms

## Data Model

Each stem is represented by a `StemData` object that contains:

- **Audio Buffer**: The isolated audio data for the stem
- **Frequency Data**: Analyzed frequency content (frequencies and magnitudes)
- **LUFS**: Loudness measurement
- **RMS**: Root Mean Square level
- **Width**: Stereo width measurement

This comprehensive data model allows for detailed analysis and comparison of stems.

## Future Implementation

The Stem Analysis module is designed to be extended with the following features in future updates:

### AI-Based Mix Suggestions

The architecture is prepared for integration with AI systems that will:

- Analyze differences between user stems and reference stems
- Provide specific EQ and processing suggestions for each stem
- Highlight problematic frequency areas
- Suggest adjustments to improve mix balance

### Advanced Analysis Features

Future versions will include:
- Transient detection and analysis
- Dynamic range visualization
- Harmonic content analysis
- Phase correlation between stems

### Extended Stem Types

Support for additional stem types will be added:
- Guitars
- Synths
- Percussion
- Ambient/FX

## Usage

### Basic Integration

```cpp
// Include the main component
#include "StemAnalysisComponent.h"

// Create an instance
ForensEQ::StemAnalysisComponent stemAnalysis;

// Add to your component hierarchy
addAndMakeVisible(stemAnalysis);

// Load a reference track
stemAnalysis.loadReferenceTrack(File("path/to/audio.wav"));
```

### Accessing Stem Data

```cpp
// Get the stem manager
auto& stemManager = stemAnalysis.getStemManager();

// Get a specific stem
StemData* kickStem = stemManager.getStem(StemType::Kick);

// Get frequency data
std::vector<float> frequencies, magnitudes;
kickStem->getFrequencyData(frequencies, magnitudes);

// Get audio data
const AudioBuffer<float>& audio = kickStem->getAudioBuffer();

// Get analysis metrics
float lufs = kickStem->getLUFS();
float rms = kickStem->getRMS();
float width = kickStem->getWidth();
```

## Building

The module uses CMake for building:

```bash
cd modules/stem_analysis
mkdir build
cd build
cmake ..
cmake --build .
```

## Dependencies

- JUCE Framework 7.0.5 or later
- C++17 compatible compiler
- CMake 3.15 or later
- Logic Pro (optional, for high-quality stem separation)
- Demucs (optional, alternative stem separation)

## License

This module is part of the ForensEQ plugin and is subject to the same licensing terms as the main project.
