#!/usr/bin/env python3

# setup_test_db.py

import subprocess

DBFARM_PATH = "/tmp/dbfarm"

subprocess.call(["monetdbd", "create", DBFARM_PATH])
subprocess.call(
    ["monetdbd", "start", DBFARM_PATH],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
)
