# Current Task

## Current Goal

Fix the Song Details screen.

---

## Current Problem

Long song titles become corrupted while scrolling.

Observed symptoms:

- title overlaps artist
- wrapped text
- duplicated text
- ghost pixels
- rendering artifacts

---

## Files Reviewed

display_manager.cpp

display_manager.h

display.cpp

display.h

main.cpp

---

## Architecture Review

Current architecture is acceptable.

The scrolling implementation is not.

Scrolling ownership is split between:

DisplayManager

and

Display

Rendering strategy is inconsistent.

---

## Known Issues

### Issue 1

Text wrapping was not explicitly disabled.

Status:

Completed.

---

### Issue 2

Scrolling ownership split across two classes.

Status:

Deferred.

This is an architectural improvement, not the primary bug.

---

### Issue 3

Mixed rendering strategy.

Current code alternates between:

clearDisplay()

partial redraw

full redraw

Likely contributing to rendering corruption.

Status:

Highest Priority.

---

### Issue 4

Viewport calculated twice.

Status:

Pending.

---

### Issue 5

Possible off-by-one error in viewport calculation.

Status:

Pending.

---

## Development Rules

Solve one issue only.

Do not combine fixes.

Do not refactor unrelated code.

Keep commits small.

Test after every issue.

---

## Next Task

Investigate and fix the mixed rendering strategy without changing project architecture.
