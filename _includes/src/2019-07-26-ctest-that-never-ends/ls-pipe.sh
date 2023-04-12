#!/bin/bash

# ls-pipe.sh

set -e

if [ $# -ne 2 ]; then
    echo "Usage: $(basename $0) process_name_1 process_name_2"
    exit 1
fi

ONE_NAME=$1
ANOTHER_NAME=$2

ONE_COMMAND="ls -l /proc/$(pgrep $ONE_NAME)/fd | grep pipe | cut -d '>' -f 2 | sort"
ANOTHER_COMMAND="ls -l /proc/$(pgrep $ANOTHER_NAME)/fd | grep pipe | cut -d '>' -f 2 | sort"

PIPE_ID=$(comm -12 <(eval $ONE_COMMAND) <(eval $ANOTHER_COMMAND) | sed 's/[^0-9]*//g')

lsof -c ctest -c monetdbd | grep -e $PIPE_ID -e COMMAND
