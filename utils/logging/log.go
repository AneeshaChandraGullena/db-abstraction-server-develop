// Package logging .
// © Copyright 2016 IBM Corp. Licensed Materials – Property of IBM.
package logging

import (
	"os"
	"sync"

	"github.com/go-kit/kit/log"
)

var globalLogger log.Logger
var once sync.Once

type serializedLogger struct {
	mtx sync.Mutex
	log.Logger
}

func (logger *serializedLogger) Log(keyvals ...interface{}) error {
	logger.mtx.Lock()
	defer logger.mtx.Unlock()
	return logger.Logger.Log(keyvals...)
}

// NewLogger returns a generic logger
func NewLogger() log.Logger {
	var logger log.Logger
	logger = log.NewLogfmtLogger(os.Stderr)
	logger = &serializedLogger{Logger: logger}
	logger = log.NewContext(logger).With("ts", log.DefaultTimestampUTC)
	return logger
}

// GlobalLogger returns a global logger if one does not exists, else return globalLogger
func GlobalLogger() log.Logger {
	once.Do(func() {
		globalLogger = log.NewLogfmtLogger(os.Stderr)
		globalLogger = &serializedLogger{Logger: globalLogger}
		globalLogger = log.NewContext(globalLogger).With("ts", log.DefaultTimestampUTC)
	})

	return globalLogger
}
