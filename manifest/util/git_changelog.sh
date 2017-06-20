#!/bin/bash
### © Copyright 2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM ###

if [ -z "$1" ]
then
    echo "Change log file not specified, exiting..."
    exit -1
fi

if [ -z "$2" ]
then
    echo "Timestamp file not specified, exiting..."
    exit -1
fi

CHANGELOG="$1"
TS=$(<"$2")

if [ -n "$CHANGELOG" ]
then
    echo "Change log file already exists.. replacing it.."
fi

echo "Gathering log since ${TS} into file ${CHANGELOG}"

# Hack to preserve new lines on command
echo "$(git --no-pager shortlog --since="${TS}" --pretty=oneline HEAD)" > "${CHANGELOG}"
