/*
 * Copyright (c) 2026 Shelly Europe Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdint.h>

namespace PLCDummy {

class Debouncer {
public:
  explicit Debouncer(uint32_t settle_ms);
  bool update(bool raw, uint32_t now_ms);
  bool state() const { return state_; }

private:
  const uint32_t settle_ms_;
  bool state_ = false;
  bool candidate_ = false;
  uint32_t since_ms_ = 0;
};

void setStatusLed(bool on);

} // namespace PLCDummy
