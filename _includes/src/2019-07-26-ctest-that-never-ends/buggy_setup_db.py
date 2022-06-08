#!/usr/bin/env python3

import subprocess
import os

DBFARM_PATH = "/tmp/dbfarm"
subprocess.call(["monetdbd", "create", DBFARM_PATH])
subprocess.call(["monetdbd", "start", DBFARM_PATH])


"""
' Debugging code
"""
os.system("pstree -p $(pgrep ctest)")
os.system("pstree -s -p $(pgrep monetdbd)")

# Command to get the Pipe ID of the given running PID's stdout (fd/1)
COMMAND_GET_PIPE_ID = (
    "ls -l /proc/%d/fd/1 | sed -n -e 's/^.*pipe:\[\(.*\)\]/\\1/p'" % os.getpid()
)
os.system(
    "lsof -c ctest -c monetdbd -c problematic_ | grep -e $(%s) -e COMMAND"
    % COMMAND_GET_PIPE_ID
)
