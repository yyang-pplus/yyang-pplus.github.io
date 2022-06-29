#!/usr/bin/env python3

# buggy_setup_db.py

import subprocess
import os

DBFARM_PATH = "/tmp/dbfarm"

subprocess.call(["monetdbd", "create", DBFARM_PATH])
subprocess.call(["monetdbd", "start", DBFARM_PATH])
