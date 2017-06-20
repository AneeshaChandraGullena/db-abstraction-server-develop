#!/bin/sh
### © Copyright 2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM ###

if [ ! $REGISTRYHOST ]; then
	REGISTRYHOST=serviceregistry.stage1.mybluemix.net
	echo "=> Using script predefined ServiceRegistry $REGISTRYHOST ..."
else
	echo "=> Using environment predefined ServiceRegistry $REGISTRYHOST ..."
fi

REGISTRYPORT=443
CERTPATH=/etc/ssl/certs
URI=/v1/serviceManifests
PATH_SO=$1

if [ ! -e $PATH_SO ]; then
    echo $PATH_SO
    echo " does not exist! \r\n"
    exit -1
fi

echo "Registering from $PATH_SO using cert in $CERTPATH to endpoint $REGISTRYHOST and port $REGISTRYPORT, URI=$URI ..."

timeout 30s ../cegtools/deploy/cegRegister put $PATH_SO $CERTPATH $REGISTRYHOST $REGISTRYPORT $URI
RETVAL="$?"
# NOTE RETVAL is an unsigned 7 bit value so will never be > 255
#echo "RETVAL is $RETVAL"

if [ $RETVAL -eq 148 ]; then
    echo "cegRegister PUT returned 404"
    echo "Inserting from $PATH_SO using cert in $CERTPATH to endpoint $REGISTRYHOST and port $REGISTRYPORT, URI=$URI ..."
    timeout 30s  ../cegtools/deploy/cegRegister post $PATH_SO $CERTPATH $REGISTRYHOST $REGISTRYPORT $URI
    RETVAL="$?"
fi

if [ $RETVAL -eq 200 ] || [ $RETVAL -eq 201 ]; then
  echo "Manifest Registered Successfully"
  . ../cegtools/scripts/logbuild.sh
elif [ $RETVAL -eq 124 ]; then
    echo "cegRegister command timed out."
    echo "Please review this machine's networking configuration"
    echo "firewall settings, and network egress routes and confirm"
    echo "that it can successfully reach $REGISTRYHOST"
    echo "on $REGISTRYPORT."
    echo "---------\n"
    echo "Sending error log to logmet"
    . ../cegtools/scripts/logbuilderror.sh
else
    echo "cegRegister returned an error"
    echo "Please see server response above for more information."
    echo "Sending error log to logmet"
    . ../cegtools/scripts/logbuilderror.sh
fi
