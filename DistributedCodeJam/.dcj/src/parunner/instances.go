package main

import (
	"fmt"
	"io"
	"os/exec"
	"sync"
)

// InstanceError is an error with an associated instance ID
type InstanceError struct {
	ID  int
	Err error
}

func (ie InstanceError) Error() string {
	return fmt.Sprintf("Error of instance %d: %v", ie.ID, ie.Err)
}

// RunInstances starts each command from cmds in an Instance and
// waits either for all of them to finish successfully or for
// the first error. In the latter case, all the rest of
// the instances are killed. All the instances are then returned
// in the slice. RunInstances additionally guarantees the following:
// * The instance slice is valid even if the error is non-nil
// * All the commands have been started before RunInstances returns
// * All the instanced have been waited on before RunInstances returns
// * If the error encountered is associated with an instance,
//   an instance of InstanceError is returned. That instance contains
//   the instance ID of the instance that caused the error.
func RunInstances(cmds []*exec.Cmd, commLog io.Writer) ([]*Instance, error) {
	var wg sync.WaitGroup
	defer wg.Wait()

	results := make(chan error, 1)
	is := make([]*Instance, len(cmds))
	for i, cmd := range cmds {
		is[i] = &Instance{
			ID:             i,
			TotalInstances: len(cmds),
			Cmd:            cmd,
			RequestChan:    make(chan *request, 1),
			ResponseChan:   make(chan *response, 1),
		}
		if err := is[i].Start(); err != nil {
			select {
			case results <- InstanceError{i, err}:
			default:
			}
			close(is[i].RequestChan)
			continue
		}
		defer is[i].Kill()
		wg.Add(1)
		go func(i int, instance *Instance) {
			err := instance.Wait()
			if err != nil {
				select {
				case results <- InstanceError{i, err}:
				default:
				}
			}
			// The instance leaves the communication channels open. We close the RequestChan
			// to signal the message router that this instance has finished. In case of an error,
			// we need to do this after possibly storing the error, so that message router's error
			// (e.g. ErrDeadlock due to the last nonblocked instance exising) doesn't override ours.
			close(instance.RequestChan)
			wg.Done()
		}(i, is[i])
	}
	wg.Add(1)
	go func() {
		requestChans := make([]<-chan *request, len(is))
		for i := range requestChans {
			requestChans[i] = is[i].RequestChan
		}
		responseChans := make([]chan<- *response, len(is))
		for i := range responseChans {
			responseChans[i] = is[i].ResponseChan
		}
		defer func() {
			for _, ch := range responseChans {
				close(ch)
			}
		}()
		err := RouteMessages(requestChans, responseChans, commLog)
		if err != nil {
			select {
			case results <- err:
			default:
			}
		}
		wg.Done()
	}()
	go func() {
		wg.Wait()
		select {
		case results <- nil:
		default:
		}
	}()
	return is, <-results
}
