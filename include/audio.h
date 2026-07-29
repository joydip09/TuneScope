#pragma once

#include <stdint.h>

class Audio {
public:
  static bool begin();

  static void update();

  static uint16_t getRMS();
};