/*
 * Copyright (c) 2026 Shelly Europe Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <plcdummy.hpp>

static PLCDummy::Debouncer button(20);

void setup() { pinMode(USER_BTN, INPUT); }

void loop() {
  PLCDummy::setStatusLed(button.update(digitalRead(USER_BTN) == LOW, millis()));
}
