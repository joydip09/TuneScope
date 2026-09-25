# CONTRIBUTING.md

> Thank you for your interest in contributing to **TuneScope**! 🎵
>
> Whether you're fixing a bug, improving documentation, or adding a new feature, your contribution is appreciated.
>
> This guide explains the development workflow, coding standards, and expectations for all contributors.

---

# Table of Contents

- Code of Conduct
- Before You Start
- Development Environment
- Project Philosophy
- Branch Strategy
- Commit Guidelines
- Pull Request Process
- Coding Standards
- Testing Requirements
- Documentation Requirements
- Reporting Bugs
- Requesting Features
- Contribution Checklist

---

# Code of Conduct

Please be respectful and constructive.

The goal is to build a welcoming community where contributors can collaborate effectively.

Disagreements should focus on technical decisions rather than individuals.

---

# Before You Start

Before making any changes, please read:

1. `README.md`
2. `AGENTS.md`
3. `CURRENT_TASK.md`

These documents explain:

- Project architecture
- Current development status
- Coding standards
- Stable modules
- Development philosophy

---

# Development Environment

## Required Software

- Visual Studio Code
- PlatformIO Extension
- Git

---

## Required Hardware

Recommended hardware:

- ESP32-S3 (N16R8)
- INMP441 Microphone
- SSD1306 or SH1106 128×64 OLED

Some documentation-only contributions do not require hardware.

---

# Project Philosophy

TuneScope follows several core principles.

## Simplicity

Choose the simplest correct solution.

---

## Modularity

Each module should have one responsibility.

Avoid tightly coupling unrelated systems.

---

## Readability

Code should be easy to understand.

Readable code is preferred over clever code.

---

## Maintainability

Every change should make future maintenance easier—not harder.

---

# Branch Strategy

Do **not** develop directly on the `main` branch.

Suggested naming:

```text
feature/audio-improvements

feature/fft-visualizer

bugfix/title-scroll

bugfix/wifi-timeout

docs/readme

refactor/display-manager
```

---

# Commit Guidelines

Use clear commit messages.

Examples:

```text
Add WAV header validation

Fix OLED title scrolling

Improve visualizer smoothing

Refactor DisplayManager rendering

Update README build instructions
```

Avoid vague messages such as:

```text
Update

Fix stuff

Changes

Misc
```

---

# Pull Request Process

Every pull request should include:

## Description

What changed?

---

## Motivation

Why was the change needed?

---

## Testing

How was it tested?

---

## Screenshots

Include screenshots or GIFs when changing the UI.

---

## Checklist

- [ ] Builds successfully
- [ ] Tested on hardware (if applicable)
- [ ] Documentation updated
- [ ] No unnecessary architectural changes

---

# Coding Standards

## Keep Modules Independent

Avoid introducing new dependencies between unrelated modules.

---

## Preserve Public Interfaces

Avoid changing public APIs unless necessary.

---

## Use Meaningful Names

Prefer descriptive variable and function names.

Example:

Good:

```cpp
recordingDurationMs
```

Poor:

```cpp
dur
```

---

## Avoid Magic Numbers

Replace numeric literals with named constants.

---

## One Responsibility Per Function

Large functions should be divided into smaller units where practical.

---

## Consistent Formatting

Follow the existing code style throughout the project.

Do not reformat unrelated files.

---

# Testing Requirements

Every functional change should be tested.

## Audio

Verify:

- I²S initialization
- RMS updates
- Recording

---

## Recognition

Verify:

- WAV generation
- HTTPS upload
- JSON parsing
- Song information

---

## Display

Verify:

- Splash screen
- Song details
- Visualizer
- Display switching
- Title scrolling

---

## Wi-Fi

Verify:

- Initial connection
- Reconnection
- Error handling

---

## Memory

Verify:

- No PSRAM leaks
- Stable heap usage
- Repeated recognition cycles

---

# Documentation Requirements

Documentation should be updated whenever changes affect:

- Public APIs
- Build instructions
- Hardware requirements
- User interface
- Configuration
- Development workflow

Files commonly updated include:

- README.md
- AGENTS.md
- CURRENT_TASK.md
- CHANGELOG.md

---

# Reporting Bugs

When reporting a bug, please include:

- Firmware version
- Hardware used
- Steps to reproduce
- Expected behavior
- Actual behavior
- Serial monitor output
- Photos or videos (if relevant)

A minimal reproducible example is greatly appreciated.

---

# Requesting Features

Feature requests should explain:

- The problem being solved
- Proposed solution
- Expected user benefit
- Possible implementation approach (optional)

Large features may be discussed before implementation to ensure they align with the project's architecture.

---

# Architecture Rules

Please preserve the existing modular architecture.

Examples:

✅ Good

- Improve Recorder without modifying Display
- Improve DisplayManager without changing Recorder
- Add a new visualizer mode inside the Visualizer module

❌ Avoid

- Mixing networking into Display
- Drawing directly from Recognizer
- Reading GPIO inside Display

---

# Stable Modules

The following modules are considered stable.

Avoid modifying them unless your contribution specifically targets them.

- Audio
- Recorder
- WAV Generator
- Display
- DisplayManager
- Visualizer
- Recognition state machine

Bug fixes are welcome.

Architectural rewrites should be discussed first.

---

# Documentation Contributions

Documentation improvements are always welcome.

Examples:

- Correct grammar
- Improve explanations
- Add diagrams
- Add screenshots
- Improve build instructions
- Expand troubleshooting

---

# Good First Issues

If you're contributing for the first time, consider:

- Improving documentation
- Fixing typos
- Adding comments where appropriate
- Improving error messages
- Expanding troubleshooting guides
- Optimizing small sections of code

These changes help familiarize you with the project before tackling larger features.

---

# Release Workflow

Major releases generally follow this process:

1. Feature complete
2. Bug fixing
3. Code cleanup
4. Documentation review
5. Hardware testing
6. Release candidate
7. Stable release

Contributors are encouraged to avoid introducing major features during the bug-fix phase of a release cycle.

---

# Contribution Checklist

Before submitting your work:

- [ ] Project builds successfully
- [ ] Code follows existing style
- [ ] Architecture is preserved
- [ ] No unnecessary refactoring
- [ ] Hardware tested (if applicable)
- [ ] Documentation updated
- [ ] Commit messages are clear
- [ ] Pull request description is complete

---

# Thank You

Every contribution—whether it's code, documentation, bug reports, testing, or ideas—helps improve TuneScope.

Thank you for helping make the project more reliable, maintainable, and useful for the embedded systems community.

Happy coding! 🚀
