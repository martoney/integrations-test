# Security Policy

## Reporting a vulnerability

Report vulnerabilities privately through
[GitHub private vulnerability reporting](https://github.com/martoney/integrations-test/security/advisories/new).
**Do not open a public issue.**

Include what is affected (SDK version or commit, Host firmware version),
how to reproduce it, and the impact you expect. We acknowledge reports within
5 working days and keep you updated in the advisory until it is resolved.

## Scope

Especially relevant: the upload path through the Shelly Host
(`tools/`), anything that lets network input reach coprocessor code, and
anything that can leave physical outputs in an unsafe state.

## Supported versions

Fixes go into the latest release. There is no backport policy.
