#!/bin/sh
### © Copyright 2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM ###

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/cegtools/usr/lib
echo LD_LIBRARY_PATH set to $LD_LIBRARY_PATH

echo $0: cegtools successfully activated
