# DRUMR - Drum-Shaping VST3 Plugin for Music Production

DRUMR is a professional aspiring drum-shaping VST3 audio effect plugin designed to add punch, warmth, and clarity to percussive sounds.
Built with JUCE 8.0.9 and Steinberg’s VST3 SDK, this plugin offers music producers and audio engineers intuitive controls for sculpting
drums in genres from classic rock to modern electronic music.
Developed by William Ashley, DRUMR is another concept peice of a budding suite of audio tools showcased at on various videos and demo songs
that can be found at [William Ashley’s portfolio](https://www.hopp.bio/william-ashley). As an aspirant hobbyist aiming to expand music
industry knowlege incuding plugins the aim is to continue to learn and grow with plugin development along side the music I am jamming.
So today I thought I would see if I can make something like a transient shaper for drums applying some common elements of percussion
sound engineering into a basic plugin. 

Support the project at [Buy Me a Coffee](https://buymeacoffee.com/williamashley) 
I also occassionally talk about how I have gone about designing or developing plugins [Medium](https://medium.com/@12264447666.williamashley).

**Keywords**: drum-shaping VST plugin, audio effect plugin, JUCE VST3 plugin, drum processing, music production, transient shaper, saturation, compressor, EQ, gate, clipper.

*Star or fork this repository to stay updated with new features and releases!*

## Overview

DRUMR enhances drum sounds with tailored digital signal processing (DSP) for various drum types, making it ideal for music production in DAWs like FL Studio, Ableton Live, or Reaper. Whether you’re shaping a punchy kick, a snappy snare, or a cohesive drum bus, DRUMR delivers professional results. Visit [William Ashley’s website](https://12264447666william.wixsite.com/williamashley) for more audio projects.

Supported drum types:
- Kick: Tight low-end sculpting, punch, and drive.
- Snare: Enhanced snap, crack, and body.
- Toms: Improved resonance and clarity.
- Hi-Hats & Cymbals: Refined presence and transient control.
- Percussion: Flexible tone shaping for claps, shakers, congas, etc.
- Bus: Group processing for cohesive drum mixes.

## Features
- **Drum Mode Selection**: Choose Kick, Snare, Tom, Hat, Perc, or Bus for optimized DSP settings.
- **High/Low-Pass Filters**: Precise frequency sculpting for clean drum tones.
- **Transient Shaper**: Independent attack and sustain controls for dynamic shaping.
- **Compressor**: Program-dependent compression with threshold, ratio, attack, release, knee, sidechain HPF, and mix controls.
- **Saturation**: Analog-style saturation (Tanh, Diode, Tape) with drive, bias, asymmetry, oversampling, and mix for warmth.
- **EQ**: Tilt and body EQ (frequency/gain) for tonal balance.
- **Noise Gate**: Threshold, ratio, attack, release, and hysteresis for noise cleanup.
- **Clipper**: Hard clipper with ceiling and lookahead for controlled distortion.
- **Mix Controls**: Dry/wet mix and auto-gain for parallel processing and level matching.
- **Resizable UI**: Custom metallic-style interface, scalable from 1200x1200 to 2000x2000 pixels.

*Note*: Multi-band processing and presets are planned for future releases. Follow updates at [William Ashley’s Medium](https://medium.com/@12264447666.williamashley).

## Build Instructions
Build your own version of DRUMR for advanced customization:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/WilliamAshley2019/DRUMR.git
   cd DRUMR

Install Dependencies:Install JUCE 8.0.9 for the audio framework.
Obtain the Steinberg VST3 SDK (included with JUCE or via Steinberg’s developer portal).
Set VST3 SDK paths in JUCE’s Projucer under global settings.

Open the Project:Use Projucer to open DrumR.jucer (not supplied but just build a standard plugin basics with dsp module and export to Visual Studio 2022 (v17+).
Alternatively, open C:\Development\JUCE\DrumR\Builds\VisualStudio2022\DrumR.sln.

Build:In Visual Studio, select VST3 configuration and x64 platform.
Clean and rebuild the solution.
Find DrumR.vst3 in C:\Development\JUCE\DrumR\Builds\VisualStudio2022\x64\Debug\VST3.

This project is not tested or built on a mac os so I can't speak to any compatibility with it in any applied way.

Requirements: Visual Studio 2022 with C++ Desktop Development workload. macOS builds need Xcode.
InstallationCopy Plugin:Windows: Move DrumR.vst3 to C:\Program Files\Common Files\VST3\ (admin permissions required).

Rescan in DAW:Open your DAW (e.g., FL Studio, Ableton Live, Reaper). This plugin has only been tested in fl studio in rapid development
1 day.

Rescan plugins via the plugin manager.

Load DRUMR:Add DRUMR to a drum track or bus.

UsageSelect Drum Mode:Use the Mode knob to select Kick, Snare, Tom, Hat, Perc, or Bus (e.g., Kick sets HPF ~25 Hz, Snare ~60 Hz).

Shape the Sound:Filters: Adjust HPF (Hz) and LPF (Hz) to remove rumble or hiss.
Transients: Tweak Attack (dB) and Sustain (dB) for punch or sustain.
Compression: Set Comp Thresh (dB), Comp Ratio, Comp Att (ms), Comp Rel (ms), Comp Knee (dB), and Comp Mix (%).
Saturation: Choose Sat Type (Tanh, Diode, Tape), adjust Drive (dB), Bias (dB), Asymmetry (%), and Sat Mix (%).
EQ: Use Tilt (dB) and Body Hz/Gain for tone shaping.
Gate: Configure Gate Thresh (dB), Gate Ratio, Gate Att (ms), Gate Rel (ms), and Gate Hyst (dB).
Clipper: Set Ceiling (dB) and Lookahead (ms) for distortion control.
Output: Adjust Dry/Wet (%), Auto Gain, and Output (dB).

Test:Load a drum loop in your DAW.
Example: For a punchy kick, select Kick, increase Attack (dB), add Tanh saturation with Drive (dB), and lower Comp Thresh (dB).

Learn more about music production at William Ashley’s portfolio. https://12264447666william.wixsite.com/williamashley

License DRUMR is released under the GPL v3 License (see LICENSE).Third-Party 
Licenses:JUCE: Licensed under GPL v3 or Commercial terms. See JUCE License. 
This project uses GPL v3 unless you hold a commercial JUCE license.
Steinberg VST3 SDK: Licensed under Steinberg’s VST3 License Agreement.

Warning: Ensure compliance with JUCE and VST3 licensing for distribution.
Legal Notices
VST is a registered trademark of Steinberg Media Technologies GmbH.

DRUMR is not affiliated with or endorsed by Steinberg Media Technologies GmbH.
JUCE usage is governed by the JUCE license.

Contributing
Contributions to DRUMR are welcome! To contribute:Fork the repository.
Create a branch: git checkout -b feature/your-feature.
Commit changes: git commit -m "Add your feature".
Push: git push origin feature/your-feature.
Submit a pull request with a clear description.

Share ideas at William Ashley’s Medium or support development at Buy Me a Coffee.
https://medium.com/@12264447666.williamashley
https://buymeacoffee.com/williamashley




Acknowledgements
JUCE for the audio framework.
Steinberg for the VST3 SDK.
The open-source audio community for inspiration.

Explore more audio projects at William Ashley’s website.IssuesReport bugs or feature requests via Issues.
Include:OS and DAW (e.g., Windows 11, FL Studio 25).
Steps to reproduce.
Screenshots/logs.


