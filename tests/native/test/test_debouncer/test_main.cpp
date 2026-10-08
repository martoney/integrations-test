/*
 * Copyright (c) 2026 Shelly Europe Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <unity.h>

#include "plcdummy.hpp"

using PLCDummy::Debouncer;

void setUp() {}
void tearDown() {}

static void test_ignores_bounce() {
  Debouncer d(20);
  TEST_ASSERT_FALSE(d.update(true, 0));
  TEST_ASSERT_FALSE(d.update(false, 5));
  TEST_ASSERT_FALSE(d.update(true, 10));
  TEST_ASSERT_FALSE(d.update(true, 29));
  TEST_ASSERT_TRUE(d.update(true, 30));
}

static void test_releases_after_settle() {
  Debouncer d(20);
  d.update(true, 0);
  d.update(true, 20);
  TEST_ASSERT_TRUE(d.update(false, 25));
  TEST_ASSERT_FALSE(d.update(false, 45));
}

static void test_survives_millis_wraparound() {
  Debouncer d(20);
  d.update(true, 0xFFFFFFF0u);
  TEST_ASSERT_TRUE(d.update(true, 0x00000004u));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_ignores_bounce);
  RUN_TEST(test_releases_after_settle);
  RUN_TEST(test_survives_millis_wraparound);
  return UNITY_END();
}
