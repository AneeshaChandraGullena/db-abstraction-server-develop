#!/bin/bash

# setup some dummy values for local build.  Jenkins should overwrite these or set and read from ENV variables
KP_SEMVER="99.99.99"
KP_COMMIT="123456789"

# build for linux machines
#env GOOS=linux GOARCH=amd64  go build -v -ldflags "-X main.semver=${KP_SEMVER} -X main.commit=${KP_COMMIT}
env GOOS=linux GOARCH=amd64  go build -v -ldflags "-X main.semver=${KP_SEMVER} -X main.commit=${KP_COMMIT}"
echo "executable for linux is in this directory"
ls -al $(basename $(pwd))
