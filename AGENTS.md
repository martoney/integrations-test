# AGENTS.md

Dummy stand-in for the PLC-36 SDK, used to study GitHub infrastructure. The
layout mirrors the real SDK so configs carry over.

## Layout

- `driver/inc/plcdummy.hpp`: public API, namespace `PLCDummy`.
- `driver/src/*.cpp`: portable code, built for the board and for native tests.
- `driver/src/*_stm32.cpp`: the only files allowed to touch hardware.
- `examples/0-base-firmware`: build smoke test.
- `tests/native`: PC-hosted Unity tests of portable code; no device.
- `tests/functional/onboard`: Unity tests that run on a board; CI only builds them.
- `tools/upload.py` + `tests/tools`: upload helper and its stdlib-only tests.
- `docs/`: documentation pages; `website/`: the Docusaurus site that renders them.

## Rules

- Keep hardware access in `*_stm32.cpp`. Portable code must build in `tests/native`.
- No dynamic allocation in runtime paths.
- Format C/C++ with the pinned clang-format (see CONTRIBUTING.md).
- Docs links between pages use relative paths with the `.md`/`.mdx` extension.

## Verification

```bash
pio run -d examples/0-base-firmware
pio test -d tests/native
pio test -d tests/functional --without-uploading --without-testing
python3 -m unittest discover -s tests/tools -v
```
