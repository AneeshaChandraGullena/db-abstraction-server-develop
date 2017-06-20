#!/bin/sh
### © Copyright 2017 IBM Corp. All Rights Reserved Licensed Materials - Property of IBM ###
if [ "$#" -ne 2 ]
then
    echo "dist_bin_deps <shared library file name> <path to copy dependencies to>"
    exit 1
fi

if [ ! -e $1 ]
then
    echo "Library $1 does not exists!" && exit 1
fi

if [ ! -d $2 ]
then
    echo "Directory $2 does not exist, creating..." && mkdir -p "$2"
fi

#Get the SO library dependencies
echo "Collecting library dependencies from build env.."
deps=$(ldd $1 | awk 'BEGIN{ORS=" "}$1~/^\//{print $1}$3~/^\//{print $3}'|sed 's/,$/\n/')

echo "Copying dependencies..."
echo $deps

for dep in $deps
do
    echo "Copying $dep to $2"
    echo "$dep"| cpio -p -dumBv --dereference "$2"
done

echo "Done!"

