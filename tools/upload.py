#!/usr/bin/env python3
# Copyright (c) 2026 Shelly Europe Ltd.
# SPDX-License-Identifier: Apache-2.0

import sys
from urllib.parse import urlsplit


def normalize_url(target):
    if "://" not in target:
        target = "ws://" + target
    parts = urlsplit(target)
    if parts.scheme not in ("ws", "wss") or not parts.hostname:
        raise ValueError(f"not a Host address: {target!r}")
    return f"{parts.scheme}://{parts.netloc}{parts.path or '/rpc'}"


if __name__ == "__main__":
    print(normalize_url(sys.argv[1]))
