# AGENTS.md

> **AI Development Guide for TuneScope**
>
> This document defines the development philosophy, architecture, coding standards, workflows, and responsibilities for all future contributors and AI coding assistants working on the TuneScope project.
>
> Every implementation should follow this guide unless explicitly instructed otherwise.

---

# Project Overview

TuneScope is a standalone ESP32-S3 music recognition device that:

- Records audio using an INMP441 I²S microphone
- Generates a WAV file in PSRAM
- Uploads the recording to the AudD Music Recognition API
- Parses the returned JSON response
- Displays song information on an SSD1306 or SH1106 OLED
- Provides a real-time audio visualizer
- Uses a modular firmware architecture

The primary goal is **clean, maintainable, production-quality embedded firmware**, not rapid feature accumulation.

---

# Project Philosophy

Every change should follow these principles.

## 1. Simplicity

Prefer the simplest solution that solves the problem correctly.

Avoid unnecessary abstractions.

---

## 2. Modularity

Every module should have one clear responsibility.

Do not mix:

- UI
- Networking
- Audio
- Recording
- Recognition

inside the same implementation.

---

## 3. Readability

Readable code is preferred over clever code.

Future maintainers should understand a module without reverse engineering it.

---

## 4. Predictability

The firmware should behave like a state machine.

State transitions should always be explicit.

---

## 5. Stability

Working modules should remain stable.

Avoid architectural changes unless they provide significant long-term benefits.

---

# Architecture Overview

Current architecture:

```text
Button
    │
    ▼
Recognizer
    │
    ▼
Recorder
    │
    ▼
Audio
    │
    ▼
WAV Generator
    │
    ▼
HTTPS Upload
    │
    ▼
AudD API
    │
    ▼
SongInfo
    │
    ▼
DisplayManager
    │
    ▼
Display
    │
    ▼
OLED
```

Dependencies should always flow downward.

Avoid circular dependencies.

---

# Module Responsibilities

## Audio

Responsible for:

- I²S initialization
- Audio capture
- RMS calculation
- Sample acquisition

Audio must **never**:

- Perform recording
- Upload data
- Draw graphics

---

## Recorder

Responsible for:

- Recording PCM
- PSRAM allocation
- Recording duration
- Buffer ownership

Recorder must **never**:

- Parse JSON
- Render UI
- Upload audio

---

## WAV Generator

Responsible only for:

- WAV header generation
- WAV buffer construction

No networking or UI logic belongs here.

---

## Recognizer

Responsible for:

- Recording workflow
- WAV generation
- HTTPS upload
- JSON parsing
- SongInfo generation
- Recognition state notifications

Recognizer must not render to the display.

---

## Display

Responsible for drawing only.

Display decides **how** something looks.

It does **not** decide **when** it appears.

---

## DisplayManager

Responsible for:

- Display state
- Screen switching
- Title scrolling
- UI timing
- Rendering decisions

DisplayManager owns the presentation layer.

---

## Visualizer

Responsible only for waveform rendering.

Must not:

- Read microphone
- Read GPIO
- Perform recognition

---

## WiFiManager

Responsible for:

- Connection management
- Reconnection
- Connection state

Networking logic belongs here.

---

## Button

Responsible for:

- Debouncing
- Press detection

Application logic belongs elsewhere.

---

# Stable Modules

The following modules are considered stable.

Avoid modifying them unless the task explicitly requires it.

- Display
- DisplayManager
- Visualizer
- Title scrolling
- OLED layouts
- Recognition state machine
- Audio recording pipeline
- WAV generation

Bug fixes are acceptable.

Architectural rewrites are discouraged.

---

# Ownership Rules

Every resource should have one owner.

Examples:

PCM buffer

Owner:

```
Recorder
```

SongInfo

Owner:

```
Recognizer
```

Display state

Owner:

```
DisplayManager
```

Avoid shared ownership.

---

# Coding Standards

## Keep Functions Small

Functions should generally perform one task.

---

## Use Meaningful Names

Prefer:

```cpp
recordingDurationMs
```

instead of:

```cpp
dur
```

---

## Prefer Constants

Magic numbers should be replaced with named constants.

---

## Avoid Global State

Use global objects only when ownership is obvious.

---

## Separate Interface and Implementation

Public declarations belong in:

```
include/
```

Implementation belongs in:

```
src/
```

---

## Preserve Public APIs

Avoid changing function signatures without necessity.

---

## Document Architectural Decisions

Complex decisions should be explained.

Simple code should remain self-explanatory.

---

# Investigation Workflow

Before modifying code:

## Step 1

Understand the reported problem.

---

## Step 2

Identify the responsible module.

---

## Step 3

Read the module completely.

---

## Step 4

Trace the execution path.

---

## Step 5

Identify the root cause.

---

## Step 6

Implement the smallest correct fix.

---

## Step 7

Evaluate possible regressions.

---

# Bug Fix Workflow

Every bug fix should answer:

1.

What is the bug?

2.

What is the root cause?

3.

Which modules are affected?

4.

Why is this solution correct?

5.

Could this introduce regressions?

6.

How should it be tested?

---

# Feature Workflow

When implementing new functionality:

1.

Understand the requirement.

2.

Identify affected modules.

3.

Avoid unnecessary architecture changes.

4.

Implement incrementally.

5.

Test thoroughly.

6.

Document changes.

---

# Refactoring Rules

Refactoring is encouraged only when it improves:

- readability
- maintainability
- modularity

Do not refactor solely for stylistic preference.

---

# Display Rules

Never call OLED drawing functions directly from:

- Recorder
- Recognizer
- WiFiManager

Display updates should flow through:

```
DisplayManager
```

---

# Recognition Rules

Recognition states should always transition through the defined state machine.

Avoid ad-hoc flags.

---

# Memory Rules

Large buffers belong in PSRAM.

Release memory promptly.

Avoid repeated allocation when unnecessary.

---

# Error Handling

Every subsystem should fail gracefully.

Examples:

- Wi-Fi unavailable
- API timeout
- Invalid JSON
- Recording failure
- PSRAM allocation failure

Never leave the firmware in an undefined state.

---

# Logging

Serial logging should be:

- concise
- useful
- consistent

Avoid excessive debug output in release builds.

---

# Performance Guidelines

Optimize only after correctness.

Current priorities:

1.

Correctness

2.

Maintainability

3.

Responsiveness

4.

Performance

---

# Testing Checklist

Every implementation should verify:

## Audio

- Microphone initializes
- RMS updates

---

## Recorder

- Recording duration
- Sample count
- PSRAM allocation

---

## WAV

- Header validity
- File size

---

## Recognition

- Upload succeeds
- JSON parsing succeeds
- SongInfo updates correctly

---

## Display

- Screen transitions
- Title scrolling
- Visualizer
- Rendering

---

## Wi-Fi

- Connect
- Disconnect
- Reconnect

---

## Memory

- Heap stability
- PSRAM stability
- No leaks

---

# Files Commonly Read

Most feature work should begin by reading:

```
main.cpp
config.h
types.h
```

Then the affected module.

---

# Files Commonly Modified

```
CURRENT_TASK.md
AGENTS.md
README.md (ONLY IF IT IS ABSOLUTELY NECESSARY TO CHANGE DOCUMENTATION. FOR NORMAL INVESTIGATION OR BUG FIX OR SIMPLE IMPLEMENTATIONS DO NOT EDIT THIS FILE.)
```

---

# Files Rarely Modified

These files should remain relatively stable:

```
pins.h
config.h
types.h
platformio.ini
```

Modify them only when necessary.

---

# Implementation Prompt Format

Future implementation prompts should include:

- Objective
- Context
- Files to modify
- Files to read
- Files not to modify
- Expected behavior
- Constraints
- Testing instructions
- Acceptance criteria

This ensures consistent AI-generated implementations.

---

# Pull Request Guidelines

Every significant change should:

- Preserve architecture
- Avoid regressions
- Compile successfully
- Be tested on hardware
- Update documentation if necessary

---

# Release Criteria

A release should satisfy:

- Stable firmware
- No known critical bugs
- Updated documentation
- Tested recognition
- Verified display behavior
- Clean build output

---

# Release Documentation

Only when asked do this. A release documentation should modify

```
CURRENT_TASK.md
AGENTS.md
README.md (DO NOT CHANGE THE FORMAT OF THIS DOCUMENTATION. ONLY ADD NEW FEATURES OR BUG FIXES DURING RELEASE.)
CHANGELOG.md (FOR NEW RELEASE ONLY.)
```

---

# Current Version

```
v1.0.0
```

Primary objective:

Deliver a stable, well-documented first public release.

---

# Long-Term Vision

Future releases may include:

- FFT visualization
- Beat detection
- OTA firmware updates
- Configuration menu
- Song history
- Offline recognition research
- Additional recognition providers

These features should build upon the existing modular architecture rather than replacing it.

---

# Final Rule

When in doubt:

**Prefer preserving a clean, modular architecture over implementing the quickest possible solution.**

Long-term maintainability is more valuable than short-term convenience.
