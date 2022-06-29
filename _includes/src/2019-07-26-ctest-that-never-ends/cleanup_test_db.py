#!/usr/bin/env python3

# cleanup_test_db.py

import subprocess

DBFARM_PATH = "/tmp/dbfarm"
subprocess.call(["monetdbd", "stop", DBFARM_PATH])
