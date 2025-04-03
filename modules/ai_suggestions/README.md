# ForensEQ AI Suggestions Module

The AI Suggestions module provides intelligent mix recommendations based on comparisons between the user's mix and loaded reference tracks. It analyzes differences in EQ balance, loudness, and stereo width to generate actionable suggestions for improving your mix.

## Features

- **Intelligent Suggestion Engine**: Analyzes differences between your mix and reference tracks to generate targeted recommendations
- **Per-Stem Analysis**: Provides specific suggestions for Kick, Snare, Bass, Vocals, and Other stems
- **Clean UI**: Displays suggestions in an expandable drawer with filtering capabilities
- **Edison Lightbulb Toggle**: Vintage-style toggle button to enable/disable suggestion visibility
- **Integration with Other Modules**: Seamlessly works with the EQ visualizer, waveform viewer, stem analysis, and loudness/width comparison modules

## How Suggestions Are Generated

The AI suggestions module uses a multi-stage analysis process to generate mix recommendations:

### 1. Data Collection

The module collects data from other ForensEQ components:
- **EQ Visualizer**: Frequency balance data for user mix and reference tracks
- **Waveform Viewer**: Timing and transient information
- **Stem Analysis**: Isolated stem audio data
- **Loudness/Width Comparison**: LUFS, RMS, and stereo width measurements

### 2. Difference Analysis

For each stem and the full mix, the module:
- Compares frequency content at key frequency bands
- Analyzes loudness differences (LUFS and RMS)
- Evaluates stereo width variations
- Examines relationships between stems (e.g., frequency masking)

### 3. Suggestion Generation

Based on the analysis, the module generates suggestions with:
- **Clear descriptions**: E.g., "Kick: Boost around 60Hz by +2dB"
- **Priority levels**: High, Medium, or Low based on the magnitude of differences
- **Color coding**: Different colors for EQ, loudness, and stereo width suggestions
- **Stem grouping**: Suggestions organized by the stem they apply to

### 4. Advanced Analysis

Beyond basic comparisons, the module also performs:
- **Frequency masking detection**: Identifies when one stem is masking another
- **Dynamic balance analysis**: Suggests improvements to the overall dynamic relationship
- **Stereo imaging recommendations**: Provides guidance on stereo field usage

## The Edison Lightbulb Toggle

The Edison-style lightbulb toggle button provides a visually appealing way to enable or disable the AI suggestions panel.

### Implementation Details

- **Vintage Aesthetic**: Designed to resemble a classic Edison lightbulb with glass bulb and visible filament
- **State Visualization**:
  - When inactive: Bulb appears dim or outlined
  - When active: Bulb glows with a warm yellow-orange filament effect
- **Animation**: Features a subtle pulsing glow effect when active
- **Tooltips**: Displays "AI Suggestions Active" or "Suggestions Hidden" based on state
- **Positioning**: Located in a prominent position for easy access

### Technical Implementation

The lightbulb is implemented as a custom JUCE button component with:
- Custom drawing routines for the bulb, filament, base, and glow
- Timer-based animation for the pulsing effect
- Toggle state management
- Tooltip handling

## Integration with Other Modules

The AI suggestions module integrates with other ForensEQ components through bridge classes:

- **EQVisualizerBridge**: Connects with the EQ visualizer to analyze frequency content and apply EQ suggestions
- **WaveformViewerBridge**: Interfaces with the waveform viewer for timing and transient analysis
- **StemAnalysisBridge**: Works with the stem analysis module to process isolated stem data
- **LoudnessWidthBridge**: Communicates with the loudness/width comparison module for level and stereo field analysis

## Future Extensibility

The AI suggestions module is designed for future expansion:

### Genre-Aware Feedback

Future versions will include:
- Genre detection algorithms
- Genre-specific suggestion profiles
- Customizable reference targets based on musical style

### User Profile Learning

Planned learning capabilities:
- Tracking which suggestions users apply most frequently
- Adapting suggestion priority based on user preferences
- Creating personalized suggestion profiles

### Advanced AI Integration

Future AI enhancements:
- Machine learning models trained on professional mixes
- Neural network analysis of mix characteristics
- Predictive suggestions based on mix context

## Usage

To use the AI suggestions module:

1. Load a reference track in ForensEQ
2. Toggle the Edison lightbulb button to show suggestions
3. View suggestions in the expandable drawer
4. Filter suggestions by stem or type using the dropdown menus
5. Apply suggestions to your mix as desired

## Technical Requirements

- JUCE Framework 6.0 or higher
- C++14 compatible compiler
- Integration with other ForensEQ modules
