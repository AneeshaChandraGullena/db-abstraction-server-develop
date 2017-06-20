// Package main wires all of the middleware and services together
// © Copyright 2016 IBM Corp. Licensed Materials – Property of IBM.
package main

import (
	"github.ibm.com/Alchemy-Key-Protect/db-abstraction-server/cmd"
)

//  semver and commit are set by build for runtime environments
var semver string
var commit string
var runtime string // not yet used

func main() {
	cmd.SetVersion(semver, commit)
	cmd.Execute()
}
