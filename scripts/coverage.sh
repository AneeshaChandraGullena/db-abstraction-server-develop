#!/usr/bin/env bash
set -ev

for d in $(go list ./... | grep -v /vendor/); do
    GO15VENDOREXPERIMENT=1 go test -cover $d
done
