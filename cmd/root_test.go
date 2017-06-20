// © Copyright 2016 IBM Corp. Licensed Materials – Property of IBM.

package cmd

import (
	"testing"
	"time"
)

func TestSetVersion(t *testing.T) {
	SetVersion("", "")
	if mainSemver != "0.0.0" {
		t.Errorf("Expected %s, received %s", "0.0.0", mainSemver)
	}

	if mainCommit != "0000" {
		t.Errorf("Expected %s, received %s", "0000", mainCommit)
	}

	SetVersion("1.1.1", "1234")
	if mainSemver != "1.1.1" {
		t.Errorf("Expected %s, received %s", "1.1.1", mainSemver)
	}

	if mainCommit != "1234" {
		t.Errorf("Expected %s, received %s", "1234", mainCommit)
	}
}

func TestRootCMD(t *testing.T) {
	mainSemver = ""
	mainCommit = ""
	deployed = true
	quit := make(chan bool)
	go func() {
		defer func() {
			if r := recover(); r != nil {
				t.Errorf("Unexpected error %s", r)
			}
		}()

		go func() {
			for {
				if done, _ := <-quit; done {
					return
				}
			}
		}()

		rootCmd.Run(nil, nil)
	}()

	// Chosen to give enough time for the goroutines above to fully run before ending the tests
	// this will ensure that tests run the same way every time.
	time.Sleep(time.Second * 2)

	quit <- true
}
