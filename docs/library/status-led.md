---
sidebar_position: 2
---

# Status LED

`PLCDummy::setStatusLed(bool on)` drives `LED_BUILTIN`. The pin is configured
as an output on the first call; no setup is needed.

:::note
This is the only hardware-facing call in the dummy SDK. Changes to it need
hardware proof, as described in `CONTRIBUTING.md`.
:::
