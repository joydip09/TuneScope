# CURRENT_TASK.md

> **Active Development Status**
>
> This document tracks the current state of the TuneScope project. It should always reflect the latest development progress and serve as the first file an AI coding assistant or contributor reads before starting work.

---

# Project

**TuneScope**

Standalone ESP32-S3 Music Recognition Device

---

# Current Version

```text
v1.0.0
```

---

# Current Phase

```text
Phase 6
Optimization & Release
```

Project Status:

```text
🟢 Active Development
```

The core functionality of TuneScope has been implemented. Current work focuses on stabilizing the firmware, fixing remaining bugs, optimizing performance, improving documentation, and preparing the first public GitHub release.

---

# Overall Progress

| Phase                              | Status         |
| ---------------------------------- | -------------- |
| Phase 1 – Hardware Bring-up        | ✅ Complete    |
| Phase 2 – Audio & Visualizer       | ✅ Complete    |
| Phase 3 – Recording Pipeline       | ✅ Complete    |
| Phase 4 – Song Recognition         | ✅ Complete    |
| Phase 5 – OLED UI & Display System | ✅ Complete    |
| Phase 6 – Optimization & Release   | 🚧 In Progress |

---

# Completed Milestones

## Hardware

- ✅ ESP32-S3 bring-up
- ✅ OLED initialization
- ✅ SSD1306 and SH1106 OLED compatibility
- ✅ INMP441 microphone
- ✅ Button input
- ✅ Wi-Fi connectivity
- ✅ PSRAM verification

---

## Audio

- ✅ I²S audio driver
- ✅ RMS calculation
- ✅ Audio normalization
- ✅ Continuous microphone sampling
- ✅ Audio buffer management

---

## Recorder

- ✅ PSRAM recording
- ✅ Fixed-duration recording
- ✅ WAV generation
- ✅ Recorder verification utilities

---

## Recognition

- ✅ HTTPS upload
- ✅ Multipart/form-data implementation
- ✅ AudD API integration
- ✅ JSON parsing
- ✅ Song metadata extraction
- ✅ Recognition state machine
- ✅ Error handling

---

## Display

- ✅ Splash screen
- ✅ Idle screen
- ✅ Recording screen
- ✅ Uploading screen
- ✅ Recognition screen
- ✅ Song details
- ✅ Automatic title scrolling
- ✅ Display mode switching
- ✅ Configurable SSD1306/SH1106 controller selection

---

## Visualizer

- ✅ RMS-based visualization
- ✅ Smoothed animation
- ✅ Center-weighted waveform
- ✅ Silence baseline
- ✅ Optimized rendering

---

## Architecture

- ✅ Modular firmware
- ✅ Observer pattern
- ✅ DisplayManager
- ✅ Independent modules
- ✅ Clear ownership model

---

# Current Objectives

The immediate goal is preparing TuneScope for its first stable public release.

Current priorities include:

1. Resolve remaining UI and visualizer bugs.
2. Improve runtime stability.
3. Optimize rendering performance.
4. Eliminate unnecessary redraws.
5. Finalize documentation.
6. Prepare GitHub repository.
7. Release Version 1.0.0.

---

# Active Work

## Firmware Optimization

Current focus:

- Improve display responsiveness
- Reduce unnecessary CPU usage
- Improve rendering efficiency
- Optimize memory usage

---

## Bug Fixes

Current work primarily involves fixing regressions rather than introducing new functionality.

All fixes should preserve the existing architecture.

---

## Documentation

The following documentation is being completed:

- README.md
- AGENTS.md
- CURRENT_TASK.md
- CONTRIBUTING.md
- CHANGELOG.md

Display controller selection is documented in `README.md`. Set
`TUNESCOPE_DISPLAY_SH1106` to `0` for SSD1306 or `1` for SH1106 in
`include/config.h`.

---

# Known Minor Issues

These issues do not prevent release but should be addressed where practical.

### Visualizer

Status:

🟡 Minor

Description:

Small visual refinements and animation tuning remain.

---

### UI Rendering

Status:

🟡 Minor

Description:

Continue reducing unnecessary screen redraws.

---

### Long-Term Stability

Status:

🟡 Testing

Description:

Extended runtime testing is still required to verify memory stability and long-duration operation.

---

# Release Checklist

## Firmware

- [ ] Resolve remaining bugs
- [ ] Final code cleanup
- [ ] Remove unused code
- [ ] Verify release build
- [ ] Confirm stable recognition pipeline

---

## Documentation

- [ ] Complete README
- [ ] Complete AGENTS
- [ ] Complete CONTRIBUTING
- [ ] Complete CHANGELOG
- [ ] Review inline code documentation

---

## Repository

- [ ] Update .gitignore
- [ ] Add LICENSE
- [ ] Add screenshots
- [ ] Add GIF demonstrations
- [ ] Add wiring diagrams
- [ ] Add repository topics
- [ ] Publish GitHub release

---

## Testing

Verify:

- [ ] Audio recording
- [ ] WAV generation
- [ ] Recognition
- [ ] OLED rendering
- [ ] Visualizer
- [ ] Wi-Fi reconnection
- [ ] Memory stability
- [ ] PSRAM usage
- [ ] Long-duration runtime

---

# Development Rules

During Phase 6:

- Avoid unnecessary architectural changes.
- Treat stable modules as production-ready.
- Fix root causes instead of symptoms.
- Preserve modularity.
- Keep public APIs stable unless required.
- Update documentation whenever functionality changes.

---

# Stable Modules

The following modules should not be modified unless the task explicitly requires it.

- Audio
- Recorder
- WAV Generator
- Display
- DisplayManager
- Visualizer
- Recognition state machine
- Song title scrolling
- OLED layouts

Bug fixes are acceptable.

Architectural rewrites are discouraged.

---

# Upcoming Features

The following features are planned after Version 1.0.0.

## Version 1.1

- FFT audio visualizer
- Oscilloscope display mode
- Peak meter
- Improved UI animations
- Additional display modes

---

## Version 1.2

- Beat detection
- Automatic recognition timer
- Settings menu
- User preferences

---

## Version 2.0

- OTA firmware updates
- Song history
- Recognition statistics
- Offline recognition research
- Additional recognition providers

---

# Current Directory Status

Core project files:

```text
README.md           ✅ Complete
AGENTS.md           ✅ Complete
CURRENT_TASK.md     ✅ Complete
CONTRIBUTING.md     ✅ Complete
CHANGELOG.md        ✅ Complete
```

---

# Next Immediate Tasks

Priority order:

1. Complete remaining documentation.
2. Resolve outstanding firmware bugs.
3. Perform extended hardware testing.
4. Capture screenshots and GIFs.
5. Prepare GitHub repository.
6. Tag and publish **v1.0.0**.

---

# Success Criteria

Version **1.0.0** will be considered complete when:

- The firmware is stable.
- Recognition works reliably.
- The OLED UI behaves consistently.
- No critical bugs remain.
- Documentation is complete.
- The repository is ready for public release.

---

# Notes for Future Contributors

Before implementing new functionality:

1. Read `README.md`.
2. Read `AGENTS.md`.
3. Review this file for the current project status.
4. Preserve the existing architecture.
5. Keep changes modular.
6. Document significant modifications.

The objective of Phase 6 is not to add major new features, but to deliver a polished, reliable, and well-documented first public release of TuneScope.
