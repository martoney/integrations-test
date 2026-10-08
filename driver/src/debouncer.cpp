/*
 * Copyright (c) 2026 Shelly Europe Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "plcdummy.hpp"

namespace PLCDummy {

Debouncer::Debouncer(uint32_t settle_ms) : settle_ms_(settle_ms) {}

bool Debouncer::update(bool raw, uint32_t now_ms) {
  if (raw != candidate_) {
    candidate_ = raw;
    since_ms_ = now_ms;
  } else if (raw != state_ && now_ms - since_ms_ >= settle_ms_) {
    state_ = raw;
  }
  return state_;
}

} // namespace PLCDummy
