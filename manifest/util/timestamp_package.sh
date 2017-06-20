#!/bin/bash
### © Copyright 2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM ###
if [ -z "$1" ]
then
    echo "Timestamp file not specified, defaulting to ../res/package_timestamp.txt"
    TS_FILE="../res/package_timestamp.txt"
else
    TS_FILE="$1"
fi

if [ -e "$TS_FILE" ]
then
    echo "Timestamp file exists, deleting.."
    rm "${TS_FILE}"
fi

echo "Creating timestamp ${TS_FILE} file."
echo $(date -u +%F-%T) > "${TS_FILE}"
