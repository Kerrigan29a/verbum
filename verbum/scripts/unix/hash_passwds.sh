#!/bin/bash

# Copyright (c) 2013 Javier Escalada Gómez
# All rights reserved



# Check arguments
if [ "$#" -ne 2 ] || ! [ -f "$1" ]; then
  echo "Usage: $0 <configuration file> <output file>" >&2
  exit 1
fi

# Get tool path
HASH_TOOL=$(dirname $0)"/../../tools/hash"

# Check tool
if ! [ -f "$HASH_TOOL" -a -x "$HASH_TOOL" ]; then
  echo "[ERROR] Unable to find: $HASH_TOOL" >&2
  exit 1
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
    # Calculate hashes
    echo "Calculating hash for: $NAME"
    CHUNK=$($HASH_TOOL -e -p $PASSWD)
    SALT=$(echo $CHUNK | cut -d: -f 1)
    HASH=$(echo $CHUNK | cut -d: -f 2)
    FORMULA=$(printf "ibase=16; %X - %X" $STOP $START)
    AMOUNT=$(echo $FORMULA | bc)
    echo "BLOCK($NAME, \"$SALT\", \"$HASH\", $AMOUNT)" >> $2
done < "$1"
