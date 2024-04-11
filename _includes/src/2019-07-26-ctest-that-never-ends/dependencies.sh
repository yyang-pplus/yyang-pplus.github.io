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

sudo wget --output-document=/etc/apt/trusted.gpg.d/monetdb.gpg https://dev.monetdb.org/downloads/MonetDB-GPG-KEY.gpg

sudo apt update

VERSION="11.37.13"

temp_dir="/tmp/$(date +%s)"
mkdir $temp_dir

pushd $temp_dir
# The order of packages matters
SELECTED_PACKAGES=(libmonetdb-client12 libmonetdb-stream13 libmonetdb20 monetdb-client monetdb5-server monetdb5-sql)
for a_package in ${SELECTED_PACKAGES[@]}; do
    PACKAGE_NAME="${a_package}_${VERSION}_amd64.deb"
    wget https://www.monetdb.org/downloads/deb/archive/$SUITE/$PACKAGE_NAME
    sudo apt --yes --allow-downgrades install "./$PACKAGE_NAME"
done
popd

rm -rf "$temp_dir"
