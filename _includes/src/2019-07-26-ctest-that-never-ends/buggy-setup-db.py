#!/usr/bin/env python3

# buggy-setup-db.py

import subprocess
import os

DBFARM_PATH = "/tmp/dbfarm"

subprocess.call(["monetdbd", "create", DBFARM_PATH])
subprocess.call(["monetdbd", "start", DBFARM_PATH])
