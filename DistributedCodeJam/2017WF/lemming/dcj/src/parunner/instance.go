package main

import (
	"errors"
	"os"
	"os/exec"
	"sync"
	"time"
)

type Instance struct {
	ID             int
	TotalInstances int
	Cmd            *exec.Cmd

	RequestChan  chan *request
	ResponseChan chan *response

	// The following fields should not be accessed until the Instance is Waited for.
	MessagesSent     int
	MessageBytesSent int
	TimeRunning      time.Duration
	TimeBlocked      time.Duration

	errOnce  sync.Once
	err      error
	waitDone chan bool
	commDone chan bool
}

func (instance *Instance) Start() error {
	instance.waitDone = make(chan bool)
	instance.commDone = make(chan bool)

	cmdr, cmdw, err := os.Pipe()
	if err != nil {
		return err
	}
	respr, respw, err := os.Pipe()
	if err != nil {
		return err
	}
	if err := startInstance(instance.Cmd, respr, cmdw); err != nil {
		return err
	}

	go func() {
		if err := instance.communicate(cmdr, respw, instance.RequestChan, instance.ResponseChan); err != nil {
			instance.errOnce.Do(func() {
				instance.err = err
			})
			instance.Cmd.Process.Kill()
		}
		cmdr.Close()
		respw.Close()
		close(instance.commDone)
	}()
	go func() {
		err := instance.Cmd.Wait()
		instance.errOnce.Do(func() {
			instance.err = err
		})
		instance.TimeRunning = instance.Cmd.ProcessState.SystemTime() + instance.Cmd.ProcessState.UserTime()
		// We are doing it this late in order to delay error reports from communicate that are
		// a result of the pipes closing (broken pipe on write pipe, EOF on read pipe). We
		// do want to ignore some of those errors (e.g. broken pipe at the very beginning, which
		// indicates that the program didn't use the communication library at all), so currently
		// we ignore all of them.
		// TODO: Do we want to ignore then also when the program has terminated with no errors?
		//       Example: program has exited in the middle of sending a message.
		respr.Close()
		cmdw.Close()
		close(instance.waitDone)
	}()
	return nil
}

func (i *Instance) Wait() error {
	<-i.waitDone
	<-i.commDone
	return i.err
}

var ErrKilled = errors.New("killed by an explicit request")

func (i *Instance) Kill() error {
	i.errOnce.Do(func() {
		i.err = ErrKilled
	})
	return i.Cmd.Process.Kill()
}
