# Contributing

Thanks for helping. Every kind of contribution is welcome: bug fixes, new
examples, docs fixes, new SDK features and even register-map or protocol
changes. Maintainers evaluate each one by hand, so this guide is mostly about
making that evaluation quick.

By participating you agree to the [Code of Conduct](CODE_OF_CONDUCT.md).
Security problems go to [private vulnerability reporting](SECURITY.md), never
to a public issue.

## Before you start

**Bug fixes, docs fixes and new examples:** just open the pull request.

**New features, API changes and behaviour changes:** start an
[Ideas discussion](https://github.com/martoney/integrations-test/discussions/categories/ideas)
first, before you write the code. Describe what you are trying to build, what
gets in your way, and what you would want instead. A rough API sketch helps
more than a specification. Wait for a maintainer's go-ahead, then build it and
link the discussion from the pull request.

A post describing something you already built arrives after the only moment
when talking could have changed it. If you have an implementation in mind,
bring it as a question: "here's how I'd do it, does that shape work?"

**What needs a discussion first?** Anything where two reasonable people could
disagree about whether it should exist or how it should behave:

- a new public class, method, parameter or macro in `driver/inc`;
- a changed default, timing, limit or error behaviour of existing API;
- anything that changes the register map or the protocol with the Shelly Host.
  These also need a Shelly Host firmware change, which only Shelly can make,
  so they need agreement on both sides before any code;
- removing or working around behaviour the code marks as deliberate.

**What does not:** a crash, wrong data, behaviour that contradicts the docs,
or a performance fix that keeps behaviour identical. Those are bugs.

Not sure? Ask in the discussion. It costs one paragraph and can save you the
whole pull request.

## Development setup

You need Python 3 and [PlatformIO Core](https://platformio.org/install/cli)
6.1.19, the version CI uses:

```bash
pip install platformio==6.1.19
git clone https://github.com/martoney/integrations-test.git
cd integrations-test
```

| Command | What it checks | Needs hardware |
|---|---|---|
| `pio run -d examples/0-base-firmware` | Firmware builds | No |
| `pio test -d tests/native` | Portable SDK logic, on your PC | No |
| `pio test -d tests/functional --without-uploading --without-testing` | On-device tests compile | No |
| `python3 -m unittest discover -s tests/tools -v` | Upload tool | No |
| `PLC36_DUT=<ip> python3 -m unittest discover -s tests/functional -v` | Full functional suite on a device | Yes |

Docs live in `docs/` and are rendered by the Docusaurus site in `website/`:

```bash
cd website
npm ci
npm start
```

## Code style

- Follow the style of the file you are changing. Read [AGENTS.md](AGENTS.md)
  for the repository layout and rules.
- C and C++ are formatted with clang-format **23.1.3** and the repository's
  `.clang-format`. CI fails on any difference. Format before you commit:

  ```bash
  pipx run --spec clang-format==23.1.3 clang-format -i <files>
  ```

  Use exactly this version; other versions format some constructs differently.
- Hardware access belongs only in the `*_stm32.cpp` files. Everything else must
  build for the native tests.
- No dynamic allocation in runtime paths.
- New source files start with the Apache-2.0 SPDX header used by existing files.

## Testing and hardware proof

Most contributors don't own a PLC-36, and most changes don't need one.

**Changes that don't touch hardware-facing code** need the firmware build and
the native tests to pass, plus new native tests for any changed logic.
Maintainers run the hardware suite before merging when they think it's needed.

**Changes that touch hardware-facing code must include hardware proof.** A bot
labels these PRs `needs-hardware-proof` automatically when they change any of:

- `driver/src/*_stm32.cpp`, `driver/src/modbus_uart.cpp`, `driver/src/ui_ws2812.cpp`
- `driver/inc/plc36_memory_map.hpp` (the register map)
- `variants/`, `boards/`, `builder/`
- the upload tool in `tools/`

What counts as proof depends on the change and is deliberately not a fixed
checklist, but these are always required:

- the **board revision** you tested on;
- the **setup**: wiring, connected peripherals, upload method (network through
  the Host or ST-Link);
- the **functional test output** relevant to the change.

Add photos or video when the behaviour is visible (LEDs, relays), and scope or
logic analyzer captures when timing or signal shape matters. Without proof the
PR stays at `review:needs-evidence`.

## Pull requests

A pull request is a handoff. A reviewer should understand what changed, why,
and how you know it works, without reconstructing your work. Fill in the
[pull request template](.github/PULL_REQUEST_TEMPLATE.md):

- **What and why:** what was wrong or missing, what happens now, why this way.
  Not a list of files; the reviewer reads the diff.
- **Affected areas:** public API, register map, examples, tooling, docs.
  "Not affected" is an answer; a blank row is not.
- **Validation:** the exact commands you ran and their results, and what you
  did not verify. A command name without a result is not evidence.
- **Hardware proof:** when required, see above.
- **Risks:** compatibility, timing, output safety, rollback.

Keep one change per pull request. Unrelated fixes in the same diff wait for
the largest one.

### Pull request title and commit messages

Pull requests are **squash merged**. The pull request title becomes the commit
subject and the description becomes its body, so `main` stays a linear history
with one commit per change.

The title follows [Conventional Commits](https://www.conventionalcommits.org/):

```
type(scope): short lowercase summary in the imperative
```

- `type`: `feat`, `fix`, `docs`, `refactor`, `perf`, `test`, `ci`, `chore`.
- `scope`: the area, such as `adc`, `modbus`, `leds`, `upload`, `docs`, `examples`.
- Say what the user gets, not what you edited:
  `fix(adc): keep readings valid while DMA restarts`, not `fix: update adc.cpp`.

The body explains the change in a few short paragraphs of plain prose: what
was wrong as a user saw it, what happens now, and why this way. It ends with
the checks you ran:

```
fix(leds): stop the front panel flickering during uploads

The LED stream was paused while the Host wrote flash, so the chain latched
garbage and flickered. The stream now keeps running and only the colour
updates pause.

Testing: native tests, functional suite on board rev 1.0.
```

### Developer Certificate of Origin

Every commit must be signed off under the
[Developer Certificate of Origin](https://developercertificate.org/), which
certifies that you have the right to submit the change under the project's
license. Add the sign-off with `-s`:

```bash
git commit -s -m "fix(adc): keep readings valid while DMA restarts"
```

This appends `Signed-off-by: Your Name <you@example.com>` using your git
identity. A check blocks pull requests with unsigned commits. To fix existing
commits, run `git rebase --signoff main` and force-push.

The project is licensed under [Apache-2.0](LICENSE). Your contributions are
licensed under the same terms.

### AI-assisted contributions

You may use AI tools. You are the author: you must understand every line you
submit, have run the validation you claim, and be able to answer review
questions about it yourself. A pull request the author cannot explain is
closed.

## Review process

Every pull request goes through three automated steps before a maintainer
looks at it:

1. **Labels.** A bot adds `area:*` and `needs-hardware-proof` by changed
   paths, and a `size:XS` to `size:XXL` label by changed lines (tests,
   lockfiles and images don't count).
2. **Parking.** A `size:XL` or larger pull request from outside the team with
   no Ideas discussion linked gets `needs-discussion` and a comment. Link the
   discussion and the label comes off on the next push or edit.
3. **Bot review.** An AI reviewer checks correctness, repository rules, the
   template and the evidence, and posts one comment with a verdict. The
   current state is shown as exactly one label:

   | Label | Meaning |
   |---|---|
   | `review:pending` | Review running |
   | `review:ready` | No blockers found; waiting for a maintainer |
   | `review:needs-evidence` | Code looks right, but validation or hardware proof is missing |
   | `review:blocked` | A concrete problem must be fixed |
   | `review:human-required` | Protocol, policy or licensing change; a maintainer decides |
   | `review:automation-failed` | The bot could not finish; a maintainer will look |

   Each push triggers a new review. The bot's verdict is advisory: a
   maintainer always reviews and merges by hand.

CI (build, native tests, tool tests, formatting) must pass and the DCO check
must be green before merge.

### Keeping pull requests and issues active

An issue or pull request with no activity for 28 days is marked `stale` and
closed 7 days later. Push, comment, or ask a maintainer for the `pinned` or
`help wanted` label to keep it open. Reopening later is fine.

## Reporting bugs

Use the [bug report form](https://github.com/martoney/integrations-test/issues/new/choose).
Include the SDK version, Host firmware version, board revision and a minimal
sketch. A bot reads every new issue, checks for duplicates, labels it and
either traces the cause or asks only what is missing.

Questions go to
[Q&A discussions](https://github.com/martoney/integrations-test/discussions/categories/q-a),
not issues. Please write in English; machine translation is fine.

## Releases

Releases are [Semantic Versioning](https://semver.org/) git tags (`v1.2.0`)
with GitHub Releases generated from the merged pull request titles. Pin a tag
in your `platformio.ini` rather than following `main`.
