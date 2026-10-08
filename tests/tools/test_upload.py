# Copyright (c) 2026 Shelly Europe Ltd.
# SPDX-License-Identifier: Apache-2.0

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools"))

from upload import normalize_url


class NormalizeUrlTest(unittest.TestCase):
    def test_bare_ip_gets_scheme_and_rpc_path(self):
        self.assertEqual(normalize_url("192.168.1.32"), "ws://192.168.1.32/rpc")

    def test_keeps_explicit_port_and_path(self):
        self.assertEqual(normalize_url("wss://host:8443/x"), "wss://host:8443/x")

    def test_rejects_non_websocket_scheme(self):
        with self.assertRaises(ValueError):
            normalize_url("http://host")


if __name__ == "__main__":
    unittest.main()
