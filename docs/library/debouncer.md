---
sidebar_position: 1
---

# Debouncer

`PLCDummy::Debouncer` reports a digital input as changed only after it has held
the new level for `settle_ms` milliseconds.

```cpp
static PLCDummy::Debouncer button(20);

void loop() {
  bool pressed = button.update(digitalRead(USER_BTN) == LOW, millis());
  PLCDummy::setStatusLed(pressed);
}
```

`update()` handles `millis()` wrapping around after about 49 days. The full
method list is in the [API reference](../reference/api.mdx).
