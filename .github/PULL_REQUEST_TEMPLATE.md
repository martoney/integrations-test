<!--
Bug fix or small improvement? Fill in the sections below and open it.

New feature, API change or behaviour change? It needs an agreed Ideas
discussion before the code. Link it here. See CONTRIBUTING.md.

PR title: Conventional Commits, e.g. "fix(adc): keep readings during DMA restart".
It becomes the squash commit subject. Every commit needs a DCO sign-off (git commit -s).
-->

## What and why

<!-- What was wrong or missing, what happens now, and why this way. Link the
     issue or Ideas discussion. Not a file list or a walk through the diff. -->

## Affected areas

<!-- One line each; "not affected" is an answer. -->

| Area | Effect |
|---|---|
| Public API (`driver/inc`) |  |
| Register map / Host protocol |  |
| Examples |  |
| Upload tool / build integration |  |
| Docs |  |

## Validation

<!-- Exact commands and their results. Say what was not verified. -->

| Check | Result |
|---|---|
| `pio run -d examples/0-base-firmware` |  |
| `pio test -d tests/native` |  |
| `python3 -m unittest discover -s tests/tools -v` |  |

## Hardware proof

<!-- Required when the PR gets the `needs-hardware-proof` label (it touches
     hardware-facing code). What counts depends on the change, but always:
     - board revision
     - test setup: wiring, connected peripherals, upload method
     - functional test output relevant to the change
     Add photos, video or scope/logic analyzer captures when behaviour is
     visible or timing-sensitive. Otherwise write "Not applicable" and why. -->

## Risks

<!-- Compatibility, timing, safety of outputs, rollback. "None identified" needs a reason. -->
