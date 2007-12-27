#!/bin/bash
if [ -z "$1" ]
then
    echo "usage: $0 destination_file" >&2
    exit 2
fi

echo -n 'Enter passphrase: '
stty -echo
read pass
stty echo

echo -n "$pass" > "$1"
chmod 600 "$1"
echo ''
echo File created.



