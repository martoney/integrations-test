# integrations-test

A dummy stand-in for the PLC-36 SDK, used to try out the GitHub
infrastructure before the real SDK moves from GitLab: CI, docs on GitHub
Pages, issue and PR bots, and the contribution process.

- Docs: https://martoney.github.io/integrations-test/
- How it all works and what the real migration needs: [MIGRATION.md](MIGRATION.md)
- Contributing: [CONTRIBUTING.md](CONTRIBUTING.md)

## Quick start

```bash
pip install platformio==6.1.19
pio run -d examples/0-base-firmware
pio test -d tests/native
```

Licensed under [Apache-2.0](LICENSE).
