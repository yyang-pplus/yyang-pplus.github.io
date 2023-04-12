#!/usr/bin/env python3

# cleanup-test-db.py

import subprocess

DBFARM_PATH = "/tmp/dbfarm"
subprocess.call(["monetdbd", "stop", DBFARM_PATH])
