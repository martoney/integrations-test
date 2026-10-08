---
sidebar_position: 2
---

# Environment Setup

Reference the SDK from your project's `platformio.ini`. Pin a release tag so
your build does not change when `main` moves:

```ini
[env:nucleo_l476rg]
platform = platformio/ststm32@20.0.0
board = nucleo_l476rg
framework = arduino
lib_deps = https://github.com/martoney/integrations-test.git#v0.1.0
```

Then build with `pio run`. See [Debouncer](library/debouncer.md) for a first
sketch.
