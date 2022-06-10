#!/bin/bash

set -e

##
# @reference    MonetDB Download Area for Debian and Ubuntu
#               https://www.monetdb.org/downloads/deb/
##

SUITE=$(lsb_release -cs)
SOURCE_LIST="/etc/apt/sources.list.d/monetdb.list"

cat << EOF | sudo tee $SOURCE_LIST
deb https://dev.monetdb.org/downloads/deb/ $SUITE monetdb
deb-src https://dev.monetdb.org/downloads/deb/ $SUITE monetdb
EOF

wget --output-document=- https://www.monetdb.org/downloads/MonetDB-GPG-KEY | sudo apt-key add -

sudo apt update

VERSION="11.35.19"

temp_dir="/tmp/$(date +%s)"
mkdir $temp_dir

pushd $temp_dir
SELECTED_PACKAGES=(monetdb5-server monetdb5-sql monetdb-client)
for a_package in ${SELECTED_PACKAGES[@]}; do
    PACKAGE_NAME="${a_package}_${VERSION}_amd64.deb"
    wget https://www.monetdb.org/downloads/deb/archive/$SUITE/$PACKAGE_NAME
    sudo apt --yes install "./$PACKAGE_NAME"
done
popd

rm -rf "$temp_dir"
