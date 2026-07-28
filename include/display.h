#pragma once

class Display {
public:
  static bool begin();

  static void clear();

  static void showSplash();

  static void showMessage(const char *message);

  static void update();
};