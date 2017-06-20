// © Copyright 2016 IBM Corp. Licensed Materials – Property of IBM.

package cmd

import (
	"bytes"
	"io"
	"os"
	"strings"
	"testing"
)

func init() {
	// Set here to avoid race condition where these are being written for the Version tests and read for the root test.
	mainSemver = "1.2.3"
	mainCommit = "12345"
}

func captureStdout(callback func()) string {
	orig := os.Stdout
	reader, writer, _ := os.Pipe()
	os.Stdout = writer

	outCh := make(chan string)
	go func() {
		var buf bytes.Buffer
		io.Copy(&buf, reader)
		outCh <- buf.String()
	}()

	callback()

	writer.Close()
	os.Stdout = orig

	return <-outCh
}

func TestVersionCMDVersion(t *testing.T) {
	version := captureStdout(func() {
		versionCmd.Run(nil, nil)
	})

	if mainSemver != strings.TrimSpace(version) {
		t.Errorf("Expected %s, received %s", mainSemver, version)
	}
}

func TestVersionCMDCommit(t *testing.T) {
	ShowCommit = true
	commit := captureStdout(func() {
		versionCmd.Run(nil, nil)
	})

	if mainCommit != strings.TrimSpace(commit) {
		t.Errorf("Expected %s, received %s", mainCommit, commit)
	}

	// Clean up
	ShowCommit = false
}
