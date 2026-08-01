# Current Features

## OLED Display

The OLED currently supports multiple display states.

- Splash Screen
- Idle / No Song
- Recording
- Song Details
- Audio Visualizer

Display states are switched independently from the recognition process.

---

## Audio Visualizer

Features:

- Live microphone amplitude
- Mirrored waveform
- Center-weighted profile
- Smoothed animation
- Silence baseline
- Low CPU usage

The visualizer consumes normalized RMS values provided by the audio subsystem.

---

## Recognition

Workflow:

Idle

↓

Recording

↓

Uploading

↓

Recognizing

↓

Song Found / Failed

↓

Idle

---

## Song Details

Displays:

- Song Title
- Artist
- Album

Supports automatic scrolling for long titles.

Current known issue:

Very long titles may clip the final character before reversing.
