# ForensEQ Loudness & Width Comparison Module

This module is responsible for comparing loudness and stereo width between the user's mix and loaded reference tracks in the ForensEQ plugin. It provides detailed analysis of both full mix and individual stems, with visual feedback and match scoring.

## Features

### Loudness Analysis
- Analyzes LUFS (Integrated, Short-Term, Momentary) and RMS for full mix and each isolated stem
- Compares user mix against reference track with visual meters
- Highlights discrepancies with descriptive text (e.g., "+2.5 LUFS too hot" or "-3 LUFS under reference")
- Color-coded feedback based on difference severity

### Stereo Width Analysis
- Calculates stereo width using correlation coefficients and mid/side level analysis
- Displays width comparison with horizontal meters
- Uses glow/halo effects to highlight when width is similar or overly narrow/wide
- Provides percentage-based width measurements for intuitive understanding

### Match Score System
- Computes match scores between user and reference for:
  - Loudness match (per stem and full mix)
  - Stereo width match (per stem and full mix)
  - Combined overall match
- Displays scores as percentages with visual indicators
- Provides descriptive match quality ratings

### Integration with Other Modules
- Connects with the EQ visualizer module to display frequency content
- Interfaces with the waveform viewer to highlight regions of interest
- Works with the stem analysis module to provide per-stem metrics
- Prepares data for future AI suggestion features

## Technical Implementation

### Loudness Calculation Methods

The module uses industry-standard loudness measurement algorithms:

1. **Integrated LUFS**: Long-term loudness measurement following ITU-R BS.1770-4 standard
   - K-weighted filtering to model human hearing
   - Gating to ignore silence and very quiet passages
   - Provides the most accurate representation of perceived loudness

2. **Short-Term LUFS**: 3-second window loudness measurement
   - More responsive to changes than integrated LUFS
   - Useful for analyzing dynamic sections

3. **Momentary LUFS**: 400ms window loudness measurement
   - Most responsive to transients and short-term changes
   - Helps identify peaks and momentary loudness issues

4. **RMS Level**: Root Mean Square amplitude measurement
   - Traditional level measurement without perceptual weighting
   - Useful for comparing with older reference material

### Width Calculation Methods

The module uses multiple methods to analyze stereo width:

1. **Correlation Coefficient**:
   - Measures phase relationship between left and right channels
   - Range from -1.0 (out of phase) to 1.0 (in phase/mono)
   - Converted to percentage for intuitive display (0% = mono, 100% = wide)

2. **Mid/Side Ratio**:
   - Calculates the ratio of side signal level to mid signal level
   - Higher values indicate wider stereo image
   - Provides insight into stereo balance

3. **Percentage Width**:
   - Normalized representation combining correlation and M/S analysis
   - Provides an intuitive 0-100% scale for users
   - Used for match scoring and visual display

### Visual Comparison System

The module uses several visual components to display comparisons:

1. **Loudness Meters**:
   - Vertical bar graphs showing user and reference levels side by side
   - Color-coded difference indicators
   - Animated transitions for smooth visual feedback
   - Separate meters for Integrated, Short-Term, Momentary LUFS and RMS

2. **Width Meters**:
   - Horizontal meters showing stereo width from mono to wide
   - Markers for user and reference width
   - Glowing effect when width values are similar
   - Animated transitions for smooth visual feedback

3. **Match Score Display**:
   - Circular indicators showing match percentages
   - Separate indicators for loudness match, width match, and combined score
   - Color-coded feedback based on match quality
   - Descriptive text for match quality

## Integration with Other Modules

### EQ Visualizer Integration
The module connects to the EQ visualizer through the `LoudnessToEQBridge` class, which:
- Converts loudness and width data to frequency data for visualization
- Allows the EQ visualizer to display the frequency content of the selected stem
- Provides match percentage data for the EQ comparison display

### Waveform Viewer Integration
The module connects to the waveform viewer through the `LoudnessToWaveformBridge` class, which:
- Provides loudness markers that can be overlaid on the waveform
- Supplies width markers for visual representation
- Identifies regions where loudness or width significantly differs between user and reference
- Enables time-based analysis of loudness and width changes

### Stem Analysis Integration
The module connects to the stem analysis module through the `LoudnessToStemBridge` class, which:
- Provides loudness and width data for all stems
- Supplies match scores for each stem
- Generates structured data for AI suggestions
- Enables per-stem comparison and analysis

## Future Development

This module is designed to be extended with:

1. **AI-based Mix Suggestions**:
   - The module already prepares structured JSON data for AI analysis
   - Future AI components can use this data to provide specific mix improvement suggestions
   - Match scores can be used to prioritize suggestions

2. **Enhanced Visualization**:
   - Additional visualization modes like spectrograms with loudness overlay
   - 3D visualization of stereo field with time-based analysis
   - More detailed region-based analysis

3. **Extended Analysis Metrics**:
   - Dynamic range analysis and comparison
   - Crest factor analysis
   - Spectral balance metrics

## Usage

To use this module in your ForensEQ plugin implementation:

1. Create an instance of `LoudnessWidthComparisonComponent`
2. Connect it to your audio processing chain to receive audio data
3. Use the `LoudnessWidthAnalyzer` to analyze audio and generate comparison results
4. Set the comparison result on the component using `setComparisonResult()`
5. Use the bridge classes to integrate with other modules

For detailed API documentation, refer to the header files in the Source directory.
