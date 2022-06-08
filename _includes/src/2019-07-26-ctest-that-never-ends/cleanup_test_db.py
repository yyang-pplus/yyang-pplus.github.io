#!/usr/bin/env python3

import subprocess


DBFARM_PATH = "/tmp/dbfarm"
subprocess.call(["monetdbd", "stop", DBFARM_PATH])
