# GitLab → GitHub: how it works, and what the real SDK needs

This repository is a dummy with the same shape as `plc-36-sdk`: portable code
plus `*_stm32.cpp` hardware files, an example, native tests, on-board tests,
an upload tool with tests, and docs. Everything below runs here for real.
The last section lists what the real migration must change.

| File | Purpose |
|---|---|
| `.github/workflows/ci.yml` | Build, native tests, tool tests, clang-format |
| `.github/workflows/release.yml` | Tag `vX.Y.Z` → GitHub Release |
| `.github/workflows/docs.yml` | Build docs on every PR; deploy to Pages from `main` |
| `.github/workflows/issue-intake.yml` + `.opencode/agent/issue-intake.md` | AI bot: triage and label new issues |
| `.github/workflows/pr-review.yml` + `.opencode/agent/pr-review.md` | AI bot: review PRs, set `review:*` verdict label |
| `.github/workflows/pr-intake.yml` + `.github/labeler.yml` | Deterministic PR labels: area, hardware, size, parking |
| `.github/workflows/label-merge-conflict.yml` | `merge-conflict` label |
| `.github/workflows/stale.yml` | Stale issues/PRs: 28 days, then close after 7 |
| `.github/workflows/sync-labels.yml` + `.github/labels.json` | Labels as code |
| `.github/ISSUE_TEMPLATE/`, `DISCUSSION_TEMPLATE/`, `PULL_REQUEST_TEMPLATE.md` | Forms contributors fill in |
| `.github/dependabot.yml` | Keeps the SHA-pinned actions and docs deps current |
| `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, `SECURITY.md`, `LICENSE` | Community files GitHub recognises |

## 1. Documentation on GitHub Pages

**How it works.** Pages live in `docs/` next to the code, so a PR that changes
the API changes its docs in the same diff. `website/` is a Docusaurus 3 site
that renders `docs/` as the whole site (`routeBasePath: '/'`).
`docs.yml` builds on every PR touching docs, and the build fails on any broken
link. A merge to `main` uploads the build and `actions/deploy-pages` publishes it.
No `gh-pages` branch, no SSH keys, no rsync. Every page has an "Edit this
page" link to the GitHub editor.

**Moving the real PLC-36 pages** (`api-docs/docs/Devices/PLC-36`, 30 files):

- **Upgrade Docusaurus 2 beta → 3.** api-docs runs 2.0.0-beta.21 (MDX 1). Docusaurus 3
  uses MDX 3, which rejects things MDX 1 allowed (HTML comments, bare `{`
  and `<` in text). `markdown.format: 'detect'` makes `.md` files plain
  CommonMark, so only `.mdx` files are strict. Of the PLC pages, only
  `reference/plc-component.mdx` is MDX.
- **Port two custom components.** `plc-component.mdx` imports `PropertyTable.jsx` and
  `ShellyAPICodeExample.jsx` from api-docs (about 320 lines total). Copy both into
  `website/src/components/`; `website/src/components/PropertyTable.jsx` shows
  that the same import path works.
- **Rewrite links.** 65 links use `/Devices/PLC-36/...md`, which becomes a relative path
  (`library/overview.md`). 12 links point at `/gen2/...` pages that stay in
  api-docs and become absolute `https://shelly-api-docs.shelly.cloud/gen2/...` URLs.
  The build fails on any link missed.
- **Keep the agent notes out of the site.** The PLC-36 `AGENTS.md` is a `draft: true`
  page. Move it to the repo's `AGENTS.md` instead.
- **Decide what stays in api-docs.** `docs/ComponentsAndServices/PLC.mdx` documents Host RPCs and
  belongs to Host firmware, so it should stay in api-docs. The SDK docs link to it.

**Not built yet:**

- **Per-PR preview sites.** GitLab deployed every MR to `preview-api-docs`. GitHub Pages hosts one
  site per repo. PRs get a build check here; for clickable previews, add
  Netlify or Cloudflare Pages (free for open source, about 10 lines of
  workflow).
- **Docs versions per release.** `docs/` matches `main`. Once releases exist, `npm run docusaurus
  docs:version 1.0.0` snapshots the docs per release.
- **Search.** Add Algolia DocSearch, free for open source, when the site is public.
- **Custom domain.** One `CNAME` setting in the Pages settings plus `url`/`baseUrl` in
  `docusaurus.config.js`.

## 2. Bots: how OpenChamber does it

OpenChamber has three layers, and so does this repo.

**Identity: a GitHub App.** Every bot workflow starts with
`actions/create-github-app-token`. It swaps the App's private key for a
short-lived token, so comments and labels come from `shelly-plc-bot[bot]`
instead of `github-actions[bot]`. Each workflow asks only for the permissions
it needs (`permission-issues: write`, ...). That limits the damage if a
prompt-injected bot misbehaves. For example, the review bot cannot merge
because it never gets `contents: write`.

**Deterministic labels: no AI.** Rules that can be computed are computed:

- `actions/labeler`: `area:*` and `needs-hardware-proof` from changed paths;
- a `github-script` step: `size:*` from changed lines, and `needs-discussion`
  for large outside PRs with no Ideas discussion linked;
- a merge-conflict labeler and the stale bot;
- issue forms: `labels: [bug]` in `bug_report.yml` labels every bug report
  before any bot runs.

**AI judgement: opencode in Actions.** Each workflow installs the opencode
CLI and runs `opencode run --agent <name> "<prompt>"`. The agent's behaviour
lives in `.opencode/agent/<name>.md`: model, tool permissions, and
instructions. Front matter whitelists shell commands (`gh`, `rg`, `cat`, `git
log`) and denies file edits. The agents use the `gh` CLI with the App token to
read issues and PRs and to post comments.

- **Issue intake.** Runs on every new issue, or when a maintainer comments
  `@shelly-plc-bot triage [focus]`. It checks for duplicates, classifies, labels, traces the cause in
  the code, and posts exactly one comment that starts with a one-line verdict
  for the maintainer. Model: Claude Haiku (cheap, high volume).
- **PR review.** Runs on every non-draft PR and push, or on
  `@shelly-plc-bot review [focus]`. It reviews correctness, `AGENTS.md`
  rules, the PR template, and hardware proof when `needs-hardware-proof` is
  set. It posts one comment ending in
  `<!-- review-meta {"head":"<sha>","verdict":"..."} -->`. The **workflow**
  parses that line and sets exactly one `review:*` label, so the label always
  belongs to a verified comment for the exact commit. Model: Claude Sonnet.

**Trust boundaries:**

- PR bots use `pull_request_target`, so fork PRs get secrets. In return, the PR's code is never
  checked out or run; the agent reads the diff through `gh` only.
- Issue and PR text goes into the prompt as data, wrapped and labelled untrusted.
- A PR that changes `AGENTS.md`, `CONTRIBUTING.md`, `.github/` or `.opencode/`
  gets `review:human-required` with no AI review, so nobody can rewrite the
  reviewer's rules and have the reviewer approve it.
- **Improvement over OpenChamber:** OpenChamber lets *anyone* comment
  `@openchamber-bot review`. Here, comment commands only run for
  `OWNER`/`MEMBER`/`COLLABORATOR`, so strangers cannot spend your API budget.
  Caveat: GitHub reports org members with *private* membership as
  `CONTRIBUTOR`, so maintainers should make their Shelly org membership public.

**Dropped from OpenChamber, add if needed:**

- the general-purpose `/oc` bot that can push commits (`anomalyco/opencode/github` action);
- `summarize` and `help` commands;
- the 15-minute re-review throttle. Here, a 60 s wait plus
  `cancel-in-progress` collapses push bursts instead;
- reproduction in CI. To let issue intake *run* native tests, add the
  PlatformIO setup steps to `issue-intake.yml` and allow `pio test -d tests/native*`.

**Cost.** Sonnet reviews are roughly $0.10–0.50 each, depending on diff size. Haiku
intake costs cents per issue. Set a monthly spend limit on the Anthropic key.

## 3. CI/CD

| GitLab (`.gitlab-ci.yml`) | GitHub (`ci.yml`) |
|---|---|
| `tags: [docker1000]` runner | `runs-on: ubuntu-latest` (free and unlimited for public repos) |
| `image: $CI_REGISTRY_IMAGE/builder:20261007` | `setup-python` + `pip install platformio==6.1.19` + `actions/cache` of `~/.platformio` |
| `test "$(pio --version)" = ...` asserts | Not needed: the pip install pins the version |
| `build-base-firmware` | `build` job: `pio run` |
| `build-onboard-tests` | `build` job: `pio test --without-uploading --without-testing` |
| `native-tests` | `native-tests` job |
| `tool-tests` | `tool-tests` job |
| JUnit report in MR | Failing test names show in the job log. If you want them inline, add `mikepenz/action-junit-report`. |
| none | `format` job: clang-format 23.1.3 |
| none | `release.yml`: tag must match `library.json` version, then a GitHub Release with notes generated from PR titles |

**Cache.** The key is a hash of every `platformio.ini`, so it changes only
when a pin changes. With a warm cache, the STM32 build takes about 1 min; a
cold one about 2 min. For the real SDK, also hash `platform.json`.

**Not migrated: hardware tests.** The functional suite needs a PLC-36 on the network. To run it
from GitHub, register a self-hosted runner on a machine next to a test device.
**Never let fork PRs reach it:** run it only by manual `workflow_dispatch` or
on `main`.

## 4. CONTRIBUTING.md decisions

| Topic | Decision |
|---|---|
| What is accepted | Everything; maintainers evaluate by hand |
| Features / API / protocol | Ideas discussion and maintainer go-ahead before code |
| Bugs, docs, examples | Open the PR directly |
| Hardware proof | Required when `needs-hardware-proof` is set: board revision, setup, functional test output; photos/video/captures when relevant |
| No-hardware changes | Build + native tests; maintainers run hardware as needed |
| Merge strategy | Squash; PR title = Conventional Commits subject; prose body ending in `Testing:` (OpenChamber style) |
| Legal | DCO sign-off (`git commit -s`), Apache-2.0 |
| Formatting | clang-format 23.1.3, `BasedOnStyle: LLVM`, `SortIncludes: Never` |
| AI contributions | Allowed; the author is responsible |
| Security | GitHub private vulnerability reporting |
| Code of Conduct | Contributor Covenant 2.1, **contact still `[INSERT CONTACT METHOD]`** |
| Releases | SemVer tags + GitHub Releases |

## 5. One-time setup (GitHub web UI)

1. **Settings → Pages:** Source = **GitHub Actions**.
2. **Settings → General → Features:** enable **Discussions**. The default categories include
   *Ideas* and *Q&A*; the slug must stay `ideas` for the discussion template.
3. **Settings → General → Pull Requests:**
   - allow **squash merging only**;
   - default message = **Pull request title and description**;
   - enable **Always suggest updating branches** and **Automatically delete head branches**.
4. **Settings → Code security:** enable **Private vulnerability reporting**,
   **Dependabot alerts**, and **Secret scanning / push protection**.
5. **Settings → Rules → Rulesets → New branch ruleset** for `main`:
   - require a PR with 1 approval and Code Owner review;
   - require status checks `format`, `build`, `native-tests`, `tool-tests` and `DCO`;
   - require linear history;
   - block force pushes.
6. **DCO check:** install the [DCO GitHub App](https://github.com/apps/dco) on the repo.
7. **Bot App:** at https://github.com/settings/apps/new create **shelly-plc-bot**:
   - Homepage = repo URL; Webhook: **uncheck Active**;
   - Repository permissions: **Contents: Read**, **Issues: Read & write**,
     **Pull requests: Read & write**, Metadata: Read;
   - Generate a private key (.pem), then **Install App** on this repository only;
   - If the name is taken, pick another and replace `shelly-plc-bot` in the two bot workflows.
8. **Settings → Secrets and variables → Actions:**
   - Variable `BOT_APP_ID` = the App ID;
   - Secret `BOT_PRIVATE_KEY` = the whole .pem file;
   - Secret `ANTHROPIC_API_KEY`.
9. **Actions → sync-labels → Run workflow** once (it also runs on every change to `labels.json`).

## 6. Findings for the real `plc-36-sdk`

1. **Toolchain drift.** `platform.json` requires `toolchain-gccarmnoneeabi ^1.120301.0`
   (GCC 12.3), but the local `~/.platformio` already resolved it to
   **1.140201.0 (GCC 14.2)**. Pin exact versions: `1.120301.0` (or
   deliberately `1.140201.0`), and `framework-cmsis` exactly instead of `~2.50700.0`.
2. **Internal base image.** `tools/docker/Dockerfile` builds `FROM gitlab.allterco.net:5050/shelly/fw/shelly-os/build-base`,
   which outsiders cannot pull. With the plain-runner CI, delete
   `tools/docker/` and the `--version` assert lines.
3. **Stale GitLab URLs.** They appear in `README.md` (install commands), `library.properties`,
   and `tools/docker/*`. `library.properties` still says `url=...pro-uni-sdk.git`.
4. **Relicensing to Apache-2.0.**
   - Shelly-owned files say `BSD-3-Clause`; change the SPDX line and `license.txt`.
   - `variants/PLC36/PeripheralPins.c`, `variant_PLC36.cpp` and `variant_PLC36.h` are
     **© STMicroelectronics** and must keep their own license; list them in a `NOTICE` file.
   - `PinNamesVar.h`, `ldscript.ld`, `tests/native/lib/emulator/*` and the
     tool files have no header; confirm their origin first.
   - `platform.json` says `"license": "BSD"`.
5. **clang-format adoption.** With this config, 23.1.3 changes 248 lines in 29 files, mostly
   version drift such as macro continuation columns and wrapped initializers.
   Land one `style: apply clang-format` commit and add its SHA to
   `.git-blame-ignore-revs` so `git blame` skips it. Exclude `variants/`
   (vendor code) with a `variants/.clang-format` containing `DisableFormat: true`.
6. **PlatformIO quirk found here.** An on-board test project nested *inside* a library it
   pulls with `lib_deps = symlink://../..` silently loses Unity
   (`unity.h: No such file`). The real SDK avoids this because it is a
   *platform*, not a library. Point test projects at `driver/src` directly
   (see `tests/functional/platformio.ini`).
7. **Release version check.** `release.yml` here checks `library.json`; the real SDK has
   to check `platform.json` and `library.properties`.
