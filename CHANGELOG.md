# Changelog

All notable changes to this project will be documented in this file.

The format is based on **Keep a Changelog** and follows **Semantic Versioning**.

---

# [Unreleased]

## Added

- SH1106 OLED display support alongside the existing SSD1306 support.
- Centralized display-controller selection through `TUNESCOPE_DISPLAY_SH1106`.

## Changed

- OLED rendering now uses a common display-driver layer for SSD1306 and SH1106
	controllers while preserving the existing 128×64 UI.

## Fixed

- Ongoing bug fixes.

---

# [1.0.0] - Initial Public Release

The first public release of **TuneScope**.

This version establishes the complete music recognition pipeline and a modular firmware architecture for future development.

---

## Added

### Core Firmware

- Initial PlatformIO project
- ESP32-S3 support
- Modular project structure
- Centralized configuration system
- Hardware abstraction

---

### Audio System

- INMP441 I²S microphone support
- Audio driver initialization
- Continuous PCM capture
- RMS calculation
- Audio normalization

---

### Recorder

- Fixed-duration recording
- PSRAM audio buffering
- Recording validation
- Buffer management

---

### WAV Generator

- WAV header generation
- PCM-to-WAV conversion
- WAV validation utilities

---

### Song Recognition

- AudD API integration
- HTTPS communication
- Multipart/form-data upload
- JSON parsing
- Song metadata extraction

---

### Recognition States

Added recognition state machine supporting:

- Idle
- Recording
- Uploading
- Recognizing
- Song Found
- Song Not Found
- Wi-Fi Error
- Upload Failed
- API Error
- Failed

---

### OLED User Interface

Added multiple display modes:

- Splash Screen
- Idle Screen
- Recording Screen
- Recognition Screen
- Song Details
- Audio Visualizer

---

### Display Features

- Display Manager
- Automatic screen switching
- OLED rendering optimization
- Static layout caching
- Automatic title scrolling

---

### Audio Visualizer

Implemented:

- Live RMS visualization
- Smoothed waveform animation
- Center-weighted rendering
- Silence baseline animation
- Optimized refresh timing

---

### Wi-Fi

- Automatic connection
- Automatic reconnection
- Connection state management
- IP reporting

---

### Input

- Recognition button
- Display mode button
- Software debouncing

---

### Architecture

Introduced:

- Observer pattern
- Modular firmware
- Clear ownership model
- Independent subsystems
- Separation of rendering and business logic

---

## Changed

- Refactored display rendering into DisplayManager.
- Improved title scrolling implementation.
- Improved visualizer rendering quality.
- Reduced unnecessary OLED redraws.
- Simplified recognition pipeline.
- Improved memory ownership throughout the firmware.

---

## Fixed

- OLED rendering artifacts
- Display state synchronization
- Recognition state transitions
- Title scrolling edge cases
- Visualizer rendering stability
- Audio buffer ownership issues
- Wi-Fi reconnection handling
- WAV generation reliability
- JSON parsing robustness
- General firmware stability improvements

---

## Performance

### Improved

- OLED rendering efficiency
- Audio visualization smoothness
- Display refresh behavior
- Recognition workflow responsiveness
- Memory allocation strategy

---

## Documentation

Added:

- Comprehensive README
- AGENTS.md
- CURRENT_TASK.md
- CONTRIBUTING.md
- CHANGELOG.md

---

## Known Limitations

Current limitations include:

- Internet connection required
- Single recognition provider
- No OTA updates
- No offline recognition
- No song history
- RMS-based visualizer (FFT planned)
- Fixed recording duration
- No persistent settings

---

# Roadmap

## Planned for Version 1.1

### Added

- FFT audio visualizer
- Oscilloscope display mode
- Peak meter
- UI improvements
- Additional display modes

---

## Planned for Version 1.2

### Added

- Beat detection
- Automatic recognition timer
- Settings menu
- User preferences

---

## Planned for Version 2.0

### Research

- Offline recognition
- Song history
- Recognition statistics
- OTA firmware updates
- Additional music recognition providers

---

# Versioning Policy

TuneScope follows **Semantic Versioning**.

## Major Version (`X.0.0`)

Incremented when introducing breaking architectural or API changes.

Examples:

- Major firmware redesign
- Incompatible configuration changes
- New project architecture

---

## Minor Version (`1.X.0`)

Incremented when adding backward-compatible functionality.

Examples:

- New display modes
- FFT visualizer
- OTA updates
- New settings

---

## Patch Version (`1.0.X`)

Incremented for backward-compatible bug fixes.

Examples:

- Rendering fixes
- Memory leak fixes
- Wi-Fi fixes
- Recognition reliability improvements

---

# Release Notes

## Version 1.0.0

**Status:** Stable

This release marks the completion of TuneScope's first development milestone.

Highlights include:

- Complete music recognition pipeline
- Modular embedded architecture
- OLED-based user interface
- Real-time audio visualization
- AudD API integration
- Comprehensive project documentation

Future development will focus on enhancing the user experience, expanding visualization capabilities, and exploring offline recognition techniques while preserving the modular architecture established in Version 1.0.0.
