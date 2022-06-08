#!/bin/bash

set -e

##
# @reference    MonetDB Download Area for Debian and Ubuntu
#               https://www.monetdb.org/downloads/deb/
##

Suite=$(lsb_release -cs)
SOURCE_LIST="/etc/apt/sources.list.d/monetdb.list"

cat << EOF | sudo tee $SOURCE_LIST
deb https://dev.monetdb.org/downloads/deb/ $Suite monetdb
deb-src https://dev.monetdb.org/downloads/deb/ $Suite monetdb
EOF

wget --output-document=- https://www.monetdb.org/downloads/MonetDB-GPG-KEY | sudo apt-key add -

sudo apt update

sudo apt --yes install monetdb5-sql monetdb-client
