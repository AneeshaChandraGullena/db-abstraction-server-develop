#!/bin/sh
### © Copyright 2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM ###

TOKEN=G0K4CHnbemvd
GUID=e90194c1-b4e3-45dd-a18a-f23f526cfdce
LOGHOST=logs.opvis.bluemix.net
LOGPORT=9091
METHOST=metrics.opvis.bluemix.net
METPORT=9095
PATH_SO=$1

if [ ! -e $PATH_SO ]; then
    echo $PATH_SO
    echo " does not exist! \r\n"
    exit -1
fi

timeout 30s ../cegtools/deploy/cegLog cegRegisterBuildError $PATH_SO /etc/ssl/certs $LOGHOST $LOGPORT $TOKEN $GUID
RETVAL="$?"
# NOTE RETVAL is an unsigned 7 bit value so will never be > 255
#echo "RETVAL is $RETVAL"

if [ $RETVAL -eq 0 ]; then
  echo "Log sent."
elif [ $RETVAL -eq 124 ]; then
    echo "cegLog command timed out."
    echo "Please review this machine's networking configuration"
    echo "firewall settings, and network egress routes and confirm"
    echo "that it can successfully reach $LOGHOST on $LOGPORT."
    echo "---------\n"
else
    echo "cegLog returned an error."
    echo "Please review the command output for more details."
    echo "---------\n"
fi
