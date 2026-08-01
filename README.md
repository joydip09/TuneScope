# TuneScope

> **A standalone ESP32-S3 music recognition device with a real-time OLED audio visualizer.**

![Version](https://img.shields.io/badge/version-v1.0.0-blue)
![Platform](https://img.shields.io/badge/platform-ESP32--S3-success)
![Framework](https://img.shields.io/badge/framework-Arduino-orange)
![IDE](https://img.shields.io/badge/IDE-PlatformIO-blueviolet)
![Language](https://img.shields.io/badge/language-C++17-informational)
![License](https://img.shields.io/badge/license-MIT-green)
![Issues](https://img.shields.io/github/issues/joydip09/TuneScope)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange)

---

## Table of Contents

- Overview
- Motivation
- Features
- Quick Start
- Hardware
- Wiring
- Software Stack
- Architecture
- Folder Structure
- Module Documentation
- Development Journey
- Hardware Challenges
- Build Guide
- Configuration
- Version History
- Roadmap
- Known Limitations
- Contributing
- License
- Credits

---

## Overview

TuneScope is a standalone embedded music recognition device built around the **ESP32-S3** microcontroller. It continuously monitors nearby audio through an **INMP441 I2S microphone**, visualizes the detected sound on a **128×64 SSD1306 OLED display**, and identifies songs using the **AudD Music Recognition API** over Wi-Fi.

Unlike smartphone applications, TuneScope is designed as a dedicated hardware device that performs audio capture, visualization, network communication, and user interaction entirely on the ESP32-S3.

The project emphasizes modular software architecture, clean code organization, responsive embedded user interfaces, and efficient use of the ESP32-S3's PSRAM for audio buffering.

---

## Motivation

Most music recognition solutions rely entirely on smartphones. While they work well, they require launching an application, granting microphone permissions, and keeping a phone nearby.

TuneScope explores a different approach:

- A dedicated always-ready recognition device
- Simple one-button interaction
- Real-time audio visualization
- Clean embedded user interface
- Modular firmware suitable for experimentation and learning

The project also serves as a practical study of embedded systems development by combining:

- Digital audio processing
- I2S peripherals
- OLED graphics
- HTTPS networking
- REST APIs
- JSON parsing
- FreeRTOS task management
- Modular firmware architecture

The primary goal of Version 1 is to demonstrate that an ESP32-S3 can reliably perform the complete recognition pipeline while maintaining a smooth user experience.

---

# Features

## Audio Capture

- INMP441 digital I2S microphone
- 16-bit PCM recording
- 16 kHz sampling
- 5-second recording duration
- PSRAM audio buffering

---

## Music Recognition

- HTTPS communication
- Multipart WAV upload
- AudD Music Recognition API integration
- JSON response parsing
- Song metadata extraction
- Graceful error handling

---

## OLED User Interface

Multiple display modes include:

- Splash Screen
- Idle Screen
- Recording Status
- Upload Status
- Recognition Status
- Song Details
- Audio Visualizer

Long song titles automatically scroll without affecting the rest of the layout.

---

## Audio Visualizer

The integrated visualizer includes:

- Live RMS amplitude measurement
- Smooth waveform animation
- Mirrored center-weighted design
- Silence detection
- Low CPU overhead
- Independent rendering system

The visualizer is intentionally separated from the recognition pipeline to keep the display responsive while preserving a modular architecture.

---

## Connectivity

- Wi-Fi station mode
- Automatic reconnection
- HTTPS requests
- Secure communication using `WiFiClientSecure`
- REST API integration

---

## Modular Architecture

Each subsystem is implemented independently.

Core modules include:

- Audio
- Recorder
- WAV Generator
- Recognizer
- Display
- Display Manager
- Visualizer
- WiFi Manager
- Button Manager
- Configuration

This modular design allows new functionality to be added with minimal impact on existing components.

---

# Demo

> **Screenshots and animated demonstrations will be added before the public v1.0.0 release.**

## Device

```
[ Photo Placeholder ]
```

---

## Splash Screen

```
[ GIF Placeholder ]
```

---

## Audio Visualizer

```
[ GIF Placeholder ]
```

---

## Song Recognition

```
[ GIF Placeholder ]
```

---

## OLED Interface

```
[ Screenshot Placeholder ]
```

---

## Hardware Prototype

```
[ Prototype Image Placeholder ]
```

---

# Current Status

Current project version:

```
v1.0.0
```

Current development status:

- Core firmware completed
- Audio pipeline completed
- Song recognition completed
- OLED interface completed
- Audio visualizer completed
- Recognition state machine completed

The remaining work primarily focuses on optimization, documentation, testing, and preparing the project for public release.

---

# Quick Start (5 Minutes)

## 1. Clone

git clone ...

## 2. Open

Open in VS Code + PlatformIO

## 3. Create

include/secrets.h

## 4. Fill in

WiFi

AudD API Key

## 5. Build

PlatformIO Build

## 6. Upload

PlatformIO Upload

## 7. Press REC

Recognize your first song 🎵

---

# Hardware

TuneScope is built using widely available development boards and modules, making it easy to reproduce while still showcasing a complete embedded IoT application.

The hardware was selected to balance cost, performance, and ease of development while taking advantage of the ESP32-S3's built-in Wi-Fi and PSRAM.

---

# Hardware Components

| Component                            | Purpose                           |
| ------------------------------------ | --------------------------------- |
| ESP32-S3 Dev Board (N16R8)           | Main controller                   |
| INMP441 I2S Microphone               | Audio capture                     |
| SSD1306 128×64 OLED                  | User interface                    |
| Push Button                          | User input                        |
| MAX98357A I2S Amplifier _(optional)_ | Audio output (future expansion)   |
| 4Ω 3W Speaker _(optional)_           | Audio playback (future expansion) |
| USB Type-C Cable                     | Power and programming             |

---

## ESP32-S3

The ESP32-S3 serves as the central controller for the entire system.

It is responsible for:

- Managing the I2S microphone
- Recording audio
- Allocating PSRAM buffers
- Generating WAV files
- Uploading recordings over HTTPS
- Parsing JSON responses
- Rendering the OLED interface
- Running the audio visualizer
- Managing button input
- Handling Wi-Fi connectivity

### Recommended Board

```text
ESP32-S3-WROOM-1-N16R8

16 MB Flash
8 MB PSRAM
```

The additional PSRAM allows TuneScope to store raw PCM recordings without exhausting internal RAM.

---

## INMP441 Digital Microphone

The INMP441 is an I2S MEMS microphone used to capture nearby music.

### Features

- Digital output
- Low noise
- Mono recording
- 16-bit PCM
- I2S interface
- Excellent Arduino support

TuneScope records:

```text
Sample Rate : 16 kHz
Channels    : Mono
Bit Depth   : 16-bit PCM
Duration    : 5 seconds
```

This format is uploaded directly to the recognition service after adding a standard WAV header.

---

## SSD1306 OLED Display

The OLED serves as the primary user interface.

Display resolution:

```text
128 × 64 pixels
```

Communication:

```text
I²C
```

The OLED displays:

- Splash screen
- Idle screen
- Recording status
- Upload status
- Recognition status
- Song information
- Error messages
- Audio visualizer

Long song titles automatically scroll while the remainder of the layout remains static.

---

## Push Buttons

TuneScope currently uses two buttons.

### Recognition Button

Starts a new recognition cycle.

Workflow:

```text
Press

↓

Record Audio

↓

Upload

↓

Recognize

↓

Display Result
```

---

### Display Mode Button

Cycles through the available display modes.

Current modes:

- Splash Screen
- Audio Visualizer
- Song Information

The display system is independent from the recognition pipeline, allowing the interface to remain responsive while recognition is in progress.

---

## MAX98357A Amplifier _(Optional)_

The MAX98357A is currently not required for Version 1.

Future versions may use it for:

- Audio playback
- Recognition feedback
- Sound effects
- Voice prompts

The firmware architecture already leaves room for future audio output support without major restructuring.

---

## Speaker _(Optional)_

A small 4Ω 3W speaker may be connected to the MAX98357A in future releases.

Planned uses include:

- Startup sounds
- Recognition confirmation
- Error tones
- Voice notifications

No audio playback is required for the current release.

---

# Power Requirements

TuneScope is designed to operate from a standard USB connection.

| Parameter                   |       Value |
| --------------------------- | ----------: |
| Input Voltage               |     5 V USB |
| ESP32 Operating Voltage     |       3.3 V |
| Typical Current             | ~180–250 mA |
| Peak Current (Wi-Fi + OLED) |     ~350 mA |

A stable USB power source is recommended during Wi-Fi transmission.

---

# Pin Mapping

## OLED Display

| OLED Pin | ESP32-S3 |
| -------- | -------- |
| VCC      | 3.3 V    |
| GND      | GND      |
| SDA      | GPIO 8   |
| SCL      | GPIO 9   |

---

## INMP441

| INMP441 Pin | ESP32-S3             |
| ----------- | -------------------- |
| VDD         | 3.3 V                |
| GND         | GND                  |
| WS          | GPIO 4               |
| SCK         | GPIO 5               |
| SD          | GPIO 6               |
| L/R         | GND _(Left Channel)_ |

---

## Buttons

| Button       | GPIO    |
| ------------ | ------- |
| Recognition  | GPIO 7  |
| Display Mode | GPIO 10 |

---

# Wiring Diagram

## OLED

```text
ESP32-S3                 SSD1306
----------------------------------------
3.3V   ----------------> VCC
GND    ----------------> GND
GPIO 8 ----------------> SDA
GPIO 9 ----------------> SCL
```

---

## INMP441

```text
ESP32-S3                 INMP441
----------------------------------------
3.3V   ----------------> VDD
GND    ----------------> GND
GPIO 4 ----------------> WS
GPIO 5 ----------------> SCK
GPIO 6 ----------------> SD
GND    ----------------> L/R
```

---

## Buttons

```text
GPIO 7  ---- Button ---- GND

GPIO 10 ---- Button ---- GND
```

Internal pull-up resistors are enabled in software, so no external resistors are required.

---

# Hardware Photos

The following media will be added before the public release.

## Prototype

```text
[ Photo Placeholder ]
```

---

## Wiring

```text
[ Wiring Photo Placeholder ]
```

---

## OLED Interface

```text
[ OLED Close-up Placeholder ]
```

---

## Complete Assembly

```text
[ Finished Device Placeholder ]
```

---

# Hardware Design Notes

The hardware intentionally remains simple.

Rather than relying on additional sensors or external processors, TuneScope demonstrates how a single ESP32-S3 can perform:

- Digital audio acquisition
- Real-time visualization
- HTTPS communication
- JSON parsing
- OLED rendering
- User interaction

while maintaining a modular firmware architecture that can be extended in future releases.

---

# Software Stack

TuneScope is built using a modern embedded software stack centered around the ESP32-S3. The project intentionally uses widely adopted libraries and frameworks to keep development approachable while maintaining a clean, modular architecture.

The firmware separates hardware drivers, application logic, networking, and user interface into independent modules. This organization makes the codebase easier to maintain, extend, and debug.

---

# Development Environment

| Component       | Technology         |
| --------------- | ------------------ |
| IDE             | Visual Studio Code |
| Extension       | PlatformIO         |
| Framework       | Arduino Framework  |
| Language        | C++17              |
| Version Control | Git                |
| Repository      | GitHub             |

PlatformIO provides a reproducible build environment, dependency management, upload tooling, and serial monitoring, making it well suited for long-term firmware development.

---

# Target Platform

| Item      | Value                               |
| --------- | ----------------------------------- |
| MCU       | ESP32-S3                            |
| Flash     | 16 MB                               |
| PSRAM     | 8 MB                                |
| Framework | Arduino                             |
| RTOS      | FreeRTOS (built into ESP32 Arduino) |

The project relies on the ESP32-S3's PSRAM to store raw PCM recordings before converting them into WAV format.

---

# PlatformIO Configuration

The project targets the ESP32-S3 development board using the Arduino framework.

Current configuration includes:

- ESP32-S3 board definition
- USB CDC enabled
- PSRAM enabled
- 921600 baud upload speed
- 115200 baud serial monitor
- Debug logging enabled during development

PlatformIO automatically manages library dependencies and simplifies firmware uploads.

---

# Arduino Framework

The firmware is written using the Arduino framework for ESP32.

Arduino provides:

- GPIO control
- I²C support
- Wi-Fi integration
- Timing utilities
- Serial debugging
- Compatibility with the ESP32 Arduino ecosystem

Lower-level functionality, such as I²S audio capture, is accessed through ESP-IDF drivers.

---

# ESP-IDF Components

Although the application uses the Arduino framework, several ESP-IDF components are used directly where lower-level hardware control is required.

Current components include:

## I²S Driver

Used by the audio subsystem to:

- Configure the INMP441 microphone
- Receive PCM samples
- Configure DMA buffers
- Perform continuous audio capture

---

## FreeRTOS

FreeRTOS is included with the ESP32 Arduino framework.

TuneScope currently uses FreeRTOS to:

- Execute the recognition workflow in a dedicated task
- Prevent long-running recognition operations from blocking the main firmware loop

The architecture allows additional tasks to be introduced in future versions if required.

---

## Heap Capabilities API

The ESP-IDF heap capability allocator is used for PSRAM allocation.

This allows:

- Large audio buffers
- WAV generation
- Reduced internal RAM usage

without changing application logic.

---

# Core Libraries

## WiFi

Provides wireless connectivity.

Responsibilities:

- Connect to access point
- Maintain connection
- Retrieve IP address
- Detect connection loss

---

## HTTPClient

Responsible for HTTPS communication with the recognition service.

Responsibilities:

- HTTP POST requests
- Multipart uploads
- Response handling
- Timeout detection

---

## WiFiClientSecure

Provides encrypted HTTPS communication.

Current implementation accepts the remote certificate using the development configuration.

Future releases may optionally support certificate pinning.

---

## ArduinoJson

Used to parse the JSON response returned by the recognition service.

Only the required metadata is extracted:

- Song title
- Artist
- Album
- Song link
- Recognition status

This minimizes memory usage while keeping the parser straightforward.

---

## Adafruit GFX

Provides:

- Primitive drawing
- Text rendering
- Graphics support
- Font handling

It serves as the graphics foundation for the OLED user interface.

---

## Adafruit SSD1306

Handles communication with the SSD1306 OLED display.

Responsibilities include:

- Display initialization
- Frame buffer management
- Screen updates

The higher-level user interface is implemented separately in the Display module.

---

# External Services

## AudD Music Recognition API

TuneScope uses the AudD REST API for music recognition.

Recognition workflow:

```text
Record Audio
      │
      ▼
Generate WAV
      │
      ▼
HTTPS POST
      │
      ▼
AudD API
      │
      ▼
JSON Response
      │
      ▼
Display Song Information
```

The firmware uploads a five-second WAV recording using a multipart/form-data request and parses the returned metadata.

The API key is stored outside the repository in `secrets.h`.

---

# Audio Format

Version 1 standardizes the entire recognition pipeline on a single audio format.

| Parameter        | Value      |
| ---------------- | ---------- |
| Sample Rate      | 16 kHz     |
| Channels         | Mono       |
| Bit Depth        | 16-bit PCM |
| Container        | WAV        |
| Recording Length | 5 seconds  |

Using one format throughout the project simplifies recording, WAV generation, debugging, and future feature development.

---

# Network Communication

Recognition requests use HTTPS.

Communication sequence:

```text
ESP32-S3

↓

Wi-Fi

↓

HTTPS

↓

AudD API

↓

JSON

↓

SongInfo
```

The firmware performs no cloud processing beyond the recognition request.

All recording, buffering, WAV generation, and display rendering occur locally on the device.

---

# Application Architecture

TuneScope is intentionally organized into independent modules with clearly defined responsibilities.

```text
                User
                  │
                  ▼
         Recognition Button
                  │
                  ▼
          Recognition Controller
                  │
        ┌─────────┴─────────┐
        │                   │
        ▼                   ▼
   Recorder             Display Manager
        │                   │
        ▼                   │
   Audio Driver             │
        │                   │
        ▼                   ▼
  WAV Generator        Display Renderer
        │                   │
        ▼                   ▼
   HTTPS Upload      OLED Interface
        │
        ▼
  AudD Recognition API
        │
        ▼
     JSON Parser
        │
        ▼
     SongInfo
```

Each layer has a single responsibility and communicates with adjacent modules through well-defined interfaces.

---

# Recognition Pipeline

The complete recognition workflow is shown below.

```text
User presses button
        │
        ▼
Start recording
        │
        ▼
Capture PCM samples
        │
        ▼
Store in PSRAM
        │
        ▼
Generate WAV
        │
        ▼
Upload via HTTPS
        │
        ▼
Receive JSON
        │
        ▼
Extract Song Information
        │
        ▼
Update Display
```

Each stage is isolated, making failures easier to diagnose and reducing coupling between subsystems.

---

# Design Principles

The software stack follows several guiding principles throughout the project.

- Modular architecture
- Single responsibility per module
- Minimal shared state
- Hardware abstraction where practical
- Separation between rendering and application logic
- Clear ownership of resources
- Readability over cleverness
- Incremental feature development
- Maintainability before optimization

These principles have guided every phase of TuneScope's development and will continue to shape future releases.

---

# Project Architecture

TuneScope is designed using a modular architecture where each subsystem has a clearly defined responsibility. Rather than placing all functionality inside `main.cpp`, each major feature is implemented as an independent module that communicates through simple interfaces.

This approach improves maintainability, reduces coupling between components, and makes future features easier to implement without introducing regressions.

---

# High-Level Architecture

```text
                           User
                             │
                             ▼
                      Recognition Button
                             │
                             ▼
                  Recognition Controller
                             │
      ┌──────────────────────┴──────────────────────┐
      │                                             │
      ▼                                             ▼
  Recorder                                   Display Manager
      │                                             │
      ▼                                             │
 Audio Driver                                       │
      │                                             │
      ▼                                             ▼
 WAV Generator                              Display Renderer
      │                                             │
      ▼                                             ▼
 HTTPS Upload                               OLED Display
      │
      ▼
 AudD Recognition API
      │
      ▼
 JSON Parser
      │
      ▼
 SongInfo
```

Each module performs one specific job and exposes only the functionality required by the rest of the application.

---

# Firmware Execution Flow

The firmware begins execution in `main.cpp`.

```text
Power On
     │
     ▼
Initialize Hardware
     │
     ▼
Initialize Wi-Fi
     │
     ▼
Initialize Display
     │
     ▼
Initialize Audio
     │
     ▼
Wait for User Input
```

Once initialized, the firmware continuously executes a lightweight main loop.

```text
Read Buttons
      │
      ▼
Update Wi-Fi
      │
      ▼
Update Display
      │
      ▼
Repeat
```

Long-running work, such as song recognition, is executed separately to keep the interface responsive.

---

# Recognition Workflow

The recognition pipeline is intentionally divided into multiple independent stages.

```text
Button Press
      │
      ▼
Recorder
      │
      ▼
PCM Samples
      │
      ▼
WAV Generator
      │
      ▼
Recognizer
      │
      ▼
HTTPS Upload
      │
      ▼
AudD API
      │
      ▼
JSON Response
      │
      ▼
SongInfo
      │
      ▼
Display Manager
      │
      ▼
OLED
```

Each stage can be tested independently.

---

# Display Workflow

The display system is independent from the recognition logic.

```text
Recognition State
        │
        ▼
DisplayManager
        │
        ├────────► Splash Screen
        │
        ├────────► Song Details
        │
        ├────────► Audio Visualizer
        │
        └────────► Status Screens
```

Separating rendering from application logic keeps the user interface simple while allowing recognition logic to evolve independently.

---

# Observer Pattern

TuneScope uses a lightweight observer pattern to decouple the recognizer from the user interface.

```text
Recognizer
      │
      │  Notify State
      ▼
DisplayManager

Recognizer
      │
      │  Notify SongInfo
      ▼
DisplayManager
```

The recognizer never draws directly to the OLED.

Instead, it publishes state changes, and the Display Manager decides how they should be presented.

This separation greatly reduces coupling between modules.

---

# Display State Machine

The OLED currently supports three primary display modes.

```text
            Splash
               │
               ▼
         Visualizer
               │
               ▼
        Song Details
               │
               └─────────────► Splash
```

The display mode button cycles through these views.

Whenever recognition begins, the firmware temporarily switches to the Song Details interface so progress and results remain visible.

---

# Recognition State Machine

Recognition follows a predictable state machine.

```text
Idle
 │
 ▼
Recording
 │
 ▼
Uploading
 │
 ▼
Recognizing
 │
 ├──────────────► Song Found
 │
 ├──────────────► Song Not Found
 │
 ├──────────────► Upload Failed
 │
 ├──────────────► API Error
 │
 ├──────────────► Wi-Fi Error
 │
 └──────────────► Failed
          │
          ▼
         Idle
```

The UI reacts to these states rather than attempting to infer progress.

---

# Folder Structure

```text
TuneScope/
│
├── include/
├── src/
├── lib/
├── data/
├── .github/
├── platformio.ini
├── README.md
├── AGENTS.md
├── CURRENT_TASK.md
├── CONTRIBUTING.md
└── CHANGELOG.md
```

---

# include/

Contains all public interfaces and shared declarations.

Every major module exposes its public API through a corresponding header file.

Current headers include:

```text
audio.h
button.h
config.h
display.h
display_manager.h
pins.h
recognizer.h
recorder.h
types.h
visualizer.h
wav.h
wifi_manager.h
```

Keeping interfaces separate from implementations reduces compile-time dependencies and improves readability.

---

# src/

Contains the implementation of every module.

Each source file corresponds to one header.

Examples include:

```text
audio.cpp
button.cpp
display.cpp
display_manager.cpp
recognizer.cpp
recorder.cpp
visualizer.cpp
wav.cpp
wifi_manager.cpp
main.cpp
```

This one-module-per-file organization makes the project easy to navigate.

---

# lib/

Reserved for reusable libraries that are specific to TuneScope.

Version 1 currently relies primarily on PlatformIO-managed dependencies, so this directory remains intentionally minimal.

---

# data/

Reserved for filesystem assets stored in flash.

Potential future uses include:

- Icons
- Fonts
- Configuration
- UI assets
- Audio samples

---

# .github/

Contains GitHub-specific resources such as:

- Issue templates
- Pull request templates
- Workflows
- Repository configuration

These files improve project maintenance but are not required for firmware execution.

---

# Configuration Files

## platformio.ini

Defines:

- Target board
- Framework
- Build flags
- Upload configuration
- Library dependencies

This file serves as the project's build configuration.

---

## secrets.h

Stores sensitive information that should never be committed to version control.

Examples include:

- Wi-Fi credentials
- API tokens

A template is provided later in this README.

---

# Data Flow

The firmware follows a straightforward flow of information.

```text
Microphone
      │
      ▼
Audio Driver
      │
      ▼
Recorder
      │
      ▼
PCM Buffer
      │
      ▼
WAV Generator
      │
      ▼
Recognizer
      │
      ▼
SongInfo
      │
      ▼
Display Manager
      │
      ▼
OLED
```

Notice that data always moves forward through the pipeline.

There are no circular dependencies between modules.

---

# Dependency Philosophy

The project follows a layered dependency model.

```text
Application Layer
        │
        ▼
Business Logic
        │
        ▼
Hardware Abstraction
        │
        ▼
ESP32 Drivers
```

Higher-level modules should never depend on implementation details of lower-level modules beyond their public interfaces.

This keeps modules replaceable and simplifies future refactoring.

---

# Architectural Goals

The architecture of TuneScope is designed around the following principles:

- One responsibility per module
- Clear ownership of resources
- Explicit interfaces
- Minimal coupling
- Predictable state transitions
- Maintainable code organization
- Easy debugging
- Scalable design for future releases

These principles allow new features—such as FFT visualization, beat detection, OTA updates, or additional display modes—to be integrated with minimal disruption to the existing firmware.

---

# Module Documentation

TuneScope is organized into independent modules, each responsible for a single aspect of the firmware. Modules communicate through clearly defined interfaces and avoid unnecessary dependencies.

This section describes the purpose, responsibilities, dependencies, and design philosophy of every major component.

---

# Audio Module

**Files**

```text
include/audio.h
src/audio.cpp
```

## Purpose

The Audio module provides low-level access to the INMP441 digital microphone through the ESP32 I²S peripheral.

It is the only module responsible for interacting directly with the ESP-IDF I²S driver.

---

## Responsibilities

- Initialize the I²S peripheral
- Configure DMA buffers
- Capture PCM audio samples
- Calculate live RMS values
- Provide microphone samples to the recorder

---

## Public Interface

```cpp
Audio::begin()

Audio::update()

Audio::getRMS()

Audio::readSamples()
```

---

## Used By

- Recorder
- Visualizer
- Display

---

## Dependencies

- ESP-IDF I²S Driver
- Config
- Pins

---

## Design Notes

The Audio module intentionally knows nothing about:

- WAV files
- Recording duration
- Song recognition
- OLED rendering

It simply provides audio data.

---

# Recorder Module

**Files**

```text
include/recorder.h
src/recorder.cpp
```

## Purpose

The Recorder module records a fixed-duration PCM audio clip into PSRAM.

It owns the recording buffer and is responsible for its lifetime.

---

## Responsibilities

- Allocate PSRAM
- Record microphone samples
- Track recording progress
- Expose recorded PCM buffer
- Release memory

---

## Public Interface

```cpp
Recorder::begin()

Recorder::startRecording()

Recorder::clear()

Recorder::pcmData()

Recorder::sampleCount()
```

---

## Dependencies

- Audio
- Config
- ESP-IDF Heap Capabilities

---

## Used By

- Recognizer

---

## Memory Ownership

The Recorder owns the PCM buffer.

No other module should allocate, resize, or free this memory.

---

## Design Notes

Keeping recording isolated makes it easy to:

- Test recording independently
- Replace the recording backend
- Support future recording formats

---

# WAV Generator

**Files**

```text
include/wav.h
src/wav.cpp
```

## Purpose

Converts raw PCM audio into a valid WAV file.

---

## Responsibilities

- Create WAV header
- Combine header and PCM
- Validate parameters
- Allocate output buffer
- Expose WAV data

---

## Public Interface

```cpp
build()

clear()

data()

sizeBytes()
```

---

## Dependencies

None besides standard memory allocation.

---

## Used By

- Recognizer

---

## Design Notes

The WAV Generator has no knowledge of:

- Wi-Fi
- HTTP
- JSON
- Song recognition

It performs only one task:

> PCM → WAV

---

# Recognizer Module

**Files**

```text
include/recognizer.h
src/recognizer.cpp
```

## Purpose

The Recognizer coordinates the complete recognition workflow.

It serves as the application's orchestration layer.

---

## Responsibilities

- Start recording
- Generate WAV
- Upload recording
- Parse JSON
- Produce SongInfo
- Notify observers

---

## Recognition Pipeline

```text
Recorder

↓

WAV Generator

↓

HTTPS Upload

↓

JSON Parser

↓

SongInfo
```

---

## Public Interface

```cpp
begin()

recognize()

setObserver()
```

---

## Dependencies

- Recorder
- WAV Generator
- WiFi
- HTTPClient
- WiFiClientSecure
- ArduinoJson

---

## Observer Notifications

The recognizer publishes:

- RecognitionState
- SongInfo

It never updates the OLED directly.

---

## Design Notes

The recognizer is intentionally unaware of:

- OLED rendering
- Display layouts
- Animations

Its only output is recognition data.

---

# Display Module

**Files**

```text
include/display.h
src/display.cpp
```

## Purpose

The Display module renders graphics on the OLED.

It contains drawing logic but no application state.

---

## Responsibilities

- Initialize OLED
- Draw text
- Draw layouts
- Draw status screens
- Draw song information
- Draw visualizer

---

## Public Interface

```cpp
begin()

showSplash()

showSongDetails()

showVisualizer()

showWiFiStatus()

showAudioLevel()
```

---

## Dependencies

- Adafruit GFX
- SSD1306 Library
- Visualizer

---

## Used By

- DisplayManager

---

## Design Notes

The Display module should never decide:

- Which screen to show
- When to switch screens

Those decisions belong to DisplayManager.

---

# DisplayManager

**Files**

```text
include/display_manager.h
src/display_manager.cpp
```

## Purpose

DisplayManager acts as the presentation controller.

It decides what should appear on the OLED.

---

## Responsibilities

- Track display mode
- Observe recognition state
- Update title scrolling
- Control rendering frequency
- Switch between UI screens

---

## Responsibilities Diagram

```text
Recognition

↓

DisplayManager

↓

Display
```

---

## Public Interface

```cpp
update()

render()

setDisplayMode()

nextDisplayMode()

setRecognitionState()

setSongInfo()
```

---

## Dependencies

- Display
- Audio
- Recognizer Observer

---

## Design Notes

This module is the heart of the UI.

Future features should integrate here rather than modifying Display directly.

---

# Visualizer Module

**Files**

```text
include/visualizer.h
src/visualizer.cpp
```

## Purpose

Generates the animated waveform displayed on the OLED.

---

## Responsibilities

- Normalize amplitude
- Smooth animation
- Draw waveform
- Draw silence baseline

---

## Public Interface

```cpp
Visualizer::draw()
```

---

## Dependencies

- Adafruit GFX
- SSD1306

---

## Design Notes

The visualizer receives normalized audio levels.

It never reads from the microphone directly.

---

# WiFi Manager

**Files**

```text
include/wifi_manager.h
src/wifi_manager.cpp
```

## Purpose

Encapsulates all Wi-Fi management.

---

## Responsibilities

- Connect to Wi-Fi
- Detect disconnects
- Retry connections
- Report connection state
- Provide IP address

---

## Public Interface

```cpp
begin()

update()

isConnected()

localIP()

state()
```

---

## Design Notes

Centralizing networking simplifies future additions such as:

- OTA updates
- Web server
- Remote configuration

---

# Button Module

**Files**

```text
include/button.h
src/button.cpp
```

## Purpose

Handles user input with software debouncing.

---

## Responsibilities

- Read button state
- Debounce input
- Detect presses
- Generate press events

---

## Public Interface

```cpp
begin()

update()

wasPressed()

isPressed()
```

---

## Design Notes

Application code should not read GPIO pins directly.

All button handling should pass through this module.

---

# Configuration Module

**Files**

```text
include/config.h
```

## Purpose

Provides centralized compile-time configuration.

---

## Responsibilities

- Sample rate
- Recording duration
- Display settings
- Wi-Fi timeouts
- Debug flags
- Version information

---

## Design Notes

Configuration values should be modified here rather than scattered throughout the firmware.

---

# Pins Module

**Files**

```text
include/pins.h
```

## Purpose

Defines hardware pin assignments.

---

## Responsibilities

- OLED pins
- I²S pins
- Button pins

Keeping pin assignments in a single location simplifies hardware revisions.

---

# Types Module

**Files**

```text
include/types.h
```

## Purpose

Contains shared data structures used throughout the firmware.

Current types include:

- `RecognitionState`
- `SongInfo`

These definitions form the common language shared between modules.

---

# Recorder Verification Module

**Files**

```text
include/recorder_verification.h
src/recorder_verification.cpp
```

## Purpose

Provides optional diagnostic tools for validating the recording pipeline during development.

---

## Responsibilities

- Verify recording duration
- Validate WAV headers
- Check PCM integrity
- Export WAV data over serial for debugging

This module is intended for development and testing rather than normal runtime operation.

---

# Main Application

**Files**

```text
src/main.cpp
```

## Purpose

Coordinates the application lifecycle.

Responsibilities include:

- Hardware initialization
- Module initialization
- Main event loop
- Starting recognition tasks
- Updating display
- Polling buttons
- Updating Wi-Fi

The application intentionally keeps `main.cpp` lightweight by delegating functionality to dedicated modules.

---

# Module Dependency Graph

```text
main.cpp
│
├── Button
├── WiFi Manager
├── DisplayManager
│     │
│     ├── Display
│     │      └── Visualizer
│     │
│     └── Audio (RMS)
│
└── Recognizer
      │
      ├── Recorder
      │      └── Audio
      │
      ├── WAV Generator
      ├── HTTP Client
      ├── ArduinoJson
      └── AudD API
```

The architecture intentionally resembles a directed graph rather than a tightly coupled mesh. Each module has a clear owner, well-defined inputs and outputs, and a limited set of dependencies, making the firmware easier to understand, debug, and extend as TuneScope evolves.

---

# Development Journey

TuneScope was developed incrementally through a series of well-defined phases. Rather than attempting to build the complete system at once, each phase introduced a single major capability while preserving the stability of previously completed work.

This iterative approach made debugging significantly easier and helped establish a solid architectural foundation before adding more complex functionality.

---

# Phase 1 — Hardware Bring-up

## Objective

Verify that every major hardware component functioned correctly before building higher-level software.

---

## Completed

- ESP32-S3 development environment
- PlatformIO project
- Serial communication
- SSD1306 OLED initialization
- INMP441 I²S microphone communication
- Wi-Fi connectivity
- Basic GPIO input

---

## Problems Encountered

### OLED not initializing

Early testing included display initialization failures caused by:

- Incorrect wiring
- Incorrect I²C pins
- Display initialization order

---

### I²S configuration issues

The microphone initially produced invalid data due to incorrect I²S configuration parameters.

Several combinations of:

- Sample rate
- Channel format
- Communication format

were tested before obtaining reliable PCM samples.

---

### PSRAM verification

Because TuneScope stores several seconds of raw audio, PSRAM availability had to be verified before implementing the recorder.

---

## Solutions

- Centralized hardware pin definitions
- Introduced configuration constants
- Verified every peripheral independently
- Confirmed PSRAM availability before continuing

---

## Lessons Learned

Building confidence in individual hardware components first greatly reduced debugging complexity in later phases.

---

# Phase 2 — Audio Processing & Visualization

## Objective

Capture live microphone data and create a responsive audio visualizer.

---

## Completed

- Continuous microphone sampling
- RMS calculation
- Audio normalization
- OLED visualizer
- Animation smoothing
- Silence detection

---

## Problems Encountered

### Unstable RMS values

Raw microphone samples fluctuated significantly, producing a noisy and unpleasant animation.

---

### Visual jitter

Directly mapping RMS values to bar heights caused excessive flickering.

---

### Poor visual balance

Uniform bar heights produced a flat appearance that lacked visual depth.

---

## Solutions

- Introduced RMS smoothing
- Added amplitude normalization
- Designed a mirrored center-weighted waveform
- Added animated baseline dots during silence
- Tuned display refresh timing

---

## Lessons Learned

Small improvements to animation smoothing had a much larger impact on perceived quality than increasing update frequency.

---

# Phase 3 — Recording Pipeline

## Objective

Capture a fixed-length audio recording suitable for music recognition.

---

## Completed

- Fixed-duration recording
- PSRAM allocation
- PCM buffering
- WAV generation
- Recorder validation utilities

---

## Problems Encountered

### Large memory requirements

A five-second recording requires significantly more memory than internal RAM can comfortably provide.

---

### WAV header generation

Generating a standards-compliant WAV file required careful construction of header fields and validation of byte counts.

---

### Buffer ownership

Clearly defining which module owned allocated memory became increasingly important as the project grew.

---

## Solutions

- Moved recording buffers into PSRAM
- Introduced a dedicated WAV Generator module
- Clearly defined ownership of recording buffers
- Added recorder verification utilities for development

---

## Lessons Learned

Separating recording from WAV generation produced a cleaner architecture and simplified debugging.

---

# Phase 4 — Song Recognition

## Objective

Upload recordings to the recognition service and retrieve song metadata.

---

## Completed

- HTTPS communication
- Multipart/form-data uploads
- AudD integration
- JSON parsing
- Song metadata extraction
- Error handling
- Recognition state machine

---

## Problems Encountered

### Multipart uploads

Constructing multipart/form-data requests on an embedded device proved more involved than expected.

---

### HTTPS memory usage

Uploading large WAV files while maintaining sufficient heap and PSRAM required careful resource management.

---

### JSON parsing

The API returns considerably more metadata than required.

Selecting only the necessary fields reduced memory usage and simplified the parsing logic.

---

### Network failures

Recognition needed to gracefully handle:

- Wi-Fi loss
- HTTP failures
- Invalid responses
- API limits
- Timeouts

---

## Solutions

- Implemented streaming multipart uploads
- Added explicit recognition states
- Introduced user-friendly status messages
- Isolated networking inside the Recognizer module

---

## Lessons Learned

Treating the recognition process as a state machine produced a more predictable user experience and simplified error handling.

---

# Phase 5 — User Interface

## Objective

Transform the firmware from a functional prototype into a polished standalone device.

---

## Completed

- Display Manager
- Splash screen
- Idle screen
- Song details layout
- Automatic title scrolling
- Display mode switching
- Rendering optimizations
- Improved visualizer
- Observer pattern integration

---

## Problems Encountered

### UI coupling

Early versions mixed recognition logic with display rendering, making future changes difficult.

---

### Long song titles

Titles longer than the OLED width either wrapped incorrectly or became unreadable.

---

### Display flickering

Frequent full-screen redraws introduced visible flicker and unnecessary work.

---

### State transitions

Recognition and display state changes occasionally conflicted, producing inconsistent behavior.

---

## Solutions

- Introduced DisplayManager
- Separated rendering from application logic
- Implemented bidirectional title scrolling
- Reduced unnecessary redraws
- Cached static UI elements
- Improved rendering efficiency

---

## Lessons Learned

A dedicated presentation layer significantly improved maintainability and reduced complexity throughout the firmware.

---

# Development Philosophy

Several principles guided every phase of TuneScope's development.

## Build Incrementally

Each phase solved one major problem before introducing the next.

---

## Preserve Stability

Previously completed functionality was treated as stable infrastructure whenever possible.

---

## Refactor When Necessary

Architectural improvements were introduced only when they simplified future development.

---

## Favor Readability

Clear, maintainable code consistently took priority over clever or highly optimized implementations.

---

## Test Continuously

Every subsystem was verified independently before becoming part of the larger application.

---

# Architectural Evolution

The project gradually evolved from a simple prototype into a modular embedded application.

```text id="k8v0sw"
Single File Prototype
          │
          ▼
Hardware Drivers
          │
          ▼
Independent Modules
          │
          ▼
Recognition Pipeline
          │
          ▼
Display Manager
          │
          ▼
Observer Pattern
          │
          ▼
Modular Firmware
```

Each stage reduced coupling and improved maintainability.

---

# Key Milestones

| Phase   | Major Achievement                              |
| ------- | ---------------------------------------------- |
| Phase 1 | Hardware initialization                        |
| Phase 2 | Audio processing & visualizer                  |
| Phase 3 | Recording & WAV generation                     |
| Phase 4 | Online music recognition                       |
| Phase 5 | Complete OLED interface and presentation layer |

---

# Looking Ahead

With the core architecture now established, future development can focus on expanding functionality rather than redesigning the firmware.

Planned improvements include:

- FFT-based visualization
- Oscilloscope display modes
- Beat detection
- OTA firmware updates
- Improved UI animations
- Additional configuration options

The goal moving forward is to build upon a stable and well-documented foundation while preserving the modular design principles established throughout the first five development phases.

---

# Hardware Challenges

Developing TuneScope required solving several hardware-specific challenges. While each issue was eventually resolved, documenting them helps future contributors understand many of the architectural decisions made throughout the project.

---

## INMP441 Configuration

### Challenge

The INMP441 initially produced inconsistent or invalid samples because of incorrect I²S configuration.

Common symptoms included:

- Very low RMS values
- Constant maximum values
- Silence despite active audio
- Distorted recordings

### Resolution

The microphone was standardized on:

```text
16 kHz
16-bit PCM
Mono
```

using the ESP32 I²S driver.

---

## PSRAM Allocation

### Challenge

Recording five seconds of 16-bit PCM audio exceeds the amount of internal RAM that should reasonably be dedicated to buffering.

### Resolution

Recording buffers and generated WAV files are allocated in PSRAM.

Internal RAM remains available for networking, graphics, and firmware execution.

---

## OLED Rendering

### Challenge

Updating the OLED too frequently introduced flicker and unnecessary CPU usage.

### Resolution

Rendering was separated from application logic.

Static UI elements are reused while only dynamic regions are updated when necessary.

---

## Long Song Titles

### Challenge

Song titles frequently exceeded the OLED width.

Simple clipping or wrapping reduced readability.

### Resolution

DisplayManager implements bidirectional scrolling while keeping the remainder of the interface static.

---

## Recognition State Synchronization

### Challenge

Recognition progress and display updates occasionally became unsynchronized during development.

### Resolution

A dedicated recognition state machine combined with the observer pattern now drives all UI updates.

---

## Visualizer Responsiveness

### Challenge

Direct RMS rendering produced noisy animation.

### Resolution

The visualizer applies normalization, smoothing, and center-weighted scaling before rendering.

---

# Build Guide

## Requirements

- ESP32-S3 development board
- USB cable
- Visual Studio Code
- PlatformIO extension

---

## Clone Repository

```bash
git clone https://github.com/<username>/TuneScope.git

cd TuneScope
```

---

## Open Project

Open the project folder using Visual Studio Code.

PlatformIO will automatically detect the project.

---

## Install Dependencies

PlatformIO automatically downloads all required libraries during the first build.

Current dependencies include:

- Adafruit GFX Library
- Adafruit SSD1306
- ArduinoJson

---

## Configure Secrets

Create a file named:

```text
include/secrets.h
```

using the template shown below.

---

## Build

```bash
PlatformIO: Build
```

or

```bash
pio run
```

---

## Upload

```bash
PlatformIO: Upload
```

or

```bash
pio run --target upload
```

---

## Monitor

```bash
PlatformIO: Monitor
```

or

```bash
pio device monitor
```

Default serial speed:

```text
115200 baud
```

---

# Configuration

Sensitive information is intentionally excluded from the repository.

Create:

```text
include/secrets.h
```

```cpp
#pragma once

constexpr char WIFI_SSID[] = "";
constexpr char WIFI_PASSWORD[] = "";

constexpr char AUDD_API_TOKEN[] = "";
```

Never commit this file to Git.

It should be added to `.gitignore`.

---

# Repository Structure

```text
TuneScope/
│
├── include/
├── src/
├── lib/
├── data/
│
├── platformio.ini
├── README.md
├── AGENTS.md
├── CURRENT_TASK.md
├── CONTRIBUTING.md
├── CHANGELOG.md
│
└── .gitignore
```

---

# Version History

## v1.0.0

### Added

- ESP32-S3 support
- INMP441 audio capture
- PSRAM recording
- WAV generation
- HTTPS uploads
- AudD integration
- JSON parsing
- Song information display
- OLED UI
- Display Manager
- Audio Visualizer
- Title scrolling
- Recognition state machine
- Wi-Fi Manager
- Button handling
- Modular firmware architecture

---

# Roadmap

## Version 1.1

Planned improvements:

- FFT audio visualizer
- Oscilloscope mode
- Peak meter
- Better visual effects
- UI refinements
- Additional display modes

---

## Version 1.2

Potential additions:

- Beat detection
- Automatic song recognition timer
- Configurable settings
- OLED menu system

---

## Version 2.0

Long-term goals:

- OTA firmware updates
- Web configuration portal
- Song history
- Recognition statistics
- Improved audio analysis
- Additional recognition providers

---

# Known Limitations

Current limitations of Version 1 include:

- Internet connection required for recognition
- No offline fingerprint database
- Single recognition provider
- Fixed recording duration
- Fixed OLED resolution
- No persistent settings storage
- No OTA firmware updates
- No local song history
- Audio visualization is RMS-based rather than frequency-based

These limitations are intentional and help keep the first public release focused and maintainable.

---

# Contributing

Contributions are welcome.

Before opening a pull request:

- Read `AGENTS.md`
- Read `CONTRIBUTING.md`
- Ensure the project builds successfully
- Preserve existing architecture
- Avoid introducing regressions
- Keep modules independent
- Document significant changes

Bug reports, feature requests, documentation improvements, and code contributions are all appreciated.

---

# Testing

Every change should be verified against the following checklist.

## Audio

- Microphone initializes
- RMS values update
- Recording completes

---

## Recognition

- WAV generation succeeds
- Upload succeeds
- JSON parsing succeeds
- Song information displays correctly

---

## Display

- Splash screen
- Song details
- Title scrolling
- Visualizer
- Mode switching

---

## Wi-Fi

- Connects correctly
- Recovers after disconnect
- Handles invalid credentials gracefully

---

## Memory

- No PSRAM leaks
- No heap growth
- Stable during repeated recognitions

---

# License

This project is released under the MIT License.

See the `LICENSE` file for details.

---

# Credits

TuneScope was developed as an educational embedded systems project exploring the intersection of digital audio processing, IoT, networking, and embedded user interface design.

Core technologies include:

- ESP32-S3
- Arduino Framework
- PlatformIO
- FreeRTOS
- ESP-IDF
- Adafruit GFX
- Adafruit SSD1306
- ArduinoJson
- AudD Music Recognition API

Special thanks to the maintainers of the open-source libraries and tools that made this project possible.

---

# Acknowledgements

TuneScope would not have been possible without the excellent open-source ecosystem surrounding the ESP32 platform.

The project builds upon the work of:

- Espressif Systems
- PlatformIO
- Adafruit
- Arduino
- ArduinoJson
- AudD

Their contributions have enabled developers around the world to build sophisticated embedded applications with accessible hardware and software.

---

# Final Notes

TuneScope is more than a music recognition device.

It is a practical demonstration of modern embedded software engineering principles, including modular architecture, layered design, clean interfaces, state-driven UI, efficient memory management, and incremental development.

The project is intentionally structured so that future features—such as FFT-based visualizations, offline recognition, OTA updates, or expanded user interfaces—can be added without requiring a major architectural redesign.

If this project helps you learn something about embedded systems, consider starring the repository and sharing your own improvements with the community.

Happy building! 🎵
