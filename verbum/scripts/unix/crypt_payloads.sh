#!/bin/bash

# Copyright (c) 2013 Javier Escalada Gómez
# All rights reserved



# Check arguments
if [ "$#" -ne 2 ] || ! [ -f "$1" ] || ! [ -f "$2" -a -x "$2" ]; then
  echo "Usage: $0 <configuration file> <target executable>" >&2
  exit 1
fi

# Get tool path
CRYPT_TOOL=$(dirname $0)"/../../tools/crypt"

# Check tool
if ! [ -f "$CRYPT_TOOL" -a -x "$CRYPT_TOOL" ]; then
  echo "[ERROR] Unable to find: $CRYPT_TOOL" >&2
  exit 1
fi

# Get base address
if [ $(uname) = "Darwin" ]; then
    BASE=0x100000000
else
    if [ $(uname -m) = "x86_64" ]; then
        BASE= 0x00400000
    else
        BASE= 0x08048000
    fi
fi

# Process each line
while read -r line; do
    # Supress comments and empty lines
    [[ -z $line || $line = \#* ]] && continue
    # Extract data
    NAME=$(echo $line | cut -d: -f 1)
    START=$(echo $line | cut -d: -f 2)
    STOP=$(echo $line | cut -d: -f 3)
    PASSWD=$(echo $line | cut -d: -f 4)
    # Crypt
    echo "Crypting $2:$NAME"
    $CRYPT_TOOL -b $BASE -s $START -t $STOP -f $2 -p $PASSWD
done < "$1"
