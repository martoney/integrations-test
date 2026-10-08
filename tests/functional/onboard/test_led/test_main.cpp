/*
 * Copyright (c) 2026 Shelly Europe Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <unity.h>

#include "plcdummy.hpp"

void setUp() {}
void tearDown() {}

static void test_status_led_follows_state() {
  PLCDummy::setStatusLed(true);
  TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_BUILTIN));
  PLCDummy::setStatusLed(false);
  TEST_ASSERT_EQUAL(LOW, digitalRead(LED_BUILTIN));
}

void setup() {
  delay(2000);
  UNITY_BEGIN();
  RUN_TEST(test_status_led_follows_state);
  UNITY_END();
}

void loop() {}
