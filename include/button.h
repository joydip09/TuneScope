#pragma once

class Button {
public:
  static void begin();
  static void update();

  static bool wasPressed();
  static bool isPressed();
};