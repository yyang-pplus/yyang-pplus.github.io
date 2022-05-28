#!/bin/bash

#
# This script installs required dependencies.
#

set -e

THIS_DIR=$(dirname "$0")
source "$THIS_DIR/utils.sh"

QuietRun pushd "$THIS_DIR"
PROJECT_ROOT_DIR=$(GetProjectRootDir)
QuietRun popd

sudo apt update

sudo apt --yes install shunit2

QuietRun pushd "$PROJECT_ROOT_DIR"
CHILD_DEPENDENCY_SCRIPTS=($(git ls-files -- 'src/*/dependencies.sh'))

for a_script in "${CHILD_DEPENDENCY_SCRIPTS[@]}"; do
    ./$a_script
done
QuietRun popd
