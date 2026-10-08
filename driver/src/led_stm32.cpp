/*
 * Copyright (c) 2026 Shelly Europe Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>

#include "plcdummy.hpp"

namespace PLCDummy {

void setStatusLed(bool on) {
  static bool configured = false;
  if (!configured) {
    pinMode(LED_BUILTIN, OUTPUT);
    configured = true;
  }
  digitalWriteFast(digitalPinToPinName(LED_BUILTIN), on ? HIGH : LOW);
}

} // namespace PLCDummy
