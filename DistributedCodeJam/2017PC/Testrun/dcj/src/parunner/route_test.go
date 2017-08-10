package main

import (
	"bytes"
	"io/ioutil"
	"sync"
	"testing"
	"time"
)

type fakeInstance struct {
	requestChan  chan *request
	responseChan chan *response
	fakeTime     time.Duration
}

func newFakeInstance() *fakeInstance {
	return &fakeInstance{
		requestChan:  make(chan *request, 1),
		responseChan: make(chan *response, 1),
	}
}

func (fi *fakeInstance) Send(destination int, contents []byte) {
	fi.fakeTime++
	fi.requestChan <- &request{
		requestType: requestSend,
		time:        fi.fakeTime,
		destination: destination,
		message:     contents,
	}
}

func (fi *fakeInstance) RecvFrom(source int) *Message {
	fi.fakeTime++
	fi.requestChan <- &request{
		requestType: requestRecv,
		time:        fi.fakeTime,
		source:      source,
	}
	resp := <-fi.responseChan
	if resp.message.SendTime > fi.fakeTime {
		fi.fakeTime = resp.message.SendTime
	}
	resp.message.SendTime = 0
	return resp.message
}

func (fi *fakeInstance) Recv() *Message {
	fi.fakeTime++
	fi.requestChan <- &request{
		requestType: requestRecvAny,
		time:        fi.fakeTime,
	}
	resp := <-fi.responseChan
	if resp.message.SendTime > fi.fakeTime {
		fi.fakeTime = resp.message.SendTime
	}
	resp.message.SendTime = 0
	return resp.message
}

func (fi *fakeInstance) Close() {
	close(fi.requestChan)
}

func setupFakes(n int) []*fakeInstance {
	fis := make([]*fakeInstance, n)
	for i := range fis {
		fis[i] = newFakeInstance()
	}
	return fis
}

func routeFakes(fis []*fakeInstance) error {
	requestChans := make([]<-chan *request, len(fis))
	responseChans := make([]chan<- *response, len(fis))
	for i, fi := range fis {
		requestChans[i] = fi.requestChan
		responseChans[i] = fi.responseChan
	}
	return RouteMessages(requestChans, responseChans, ioutil.Discard)
}

// equivalentMessages returns true if the two messages are equal or differ in the SendTime only
func equivalentMessages(a, b *Message) bool {
	return a.Source == b.Source && a.Target == b.Target && bytes.Equal(a.Message, b.Message)
}

func TestRouterSimple(t *testing.T) {
	fakes := setupFakes(2)
	done := make(chan bool)
	go func() {
		if err := routeFakes(fakes); err != nil {
			t.Errorf("RouteMessages unexcpectedly failed: %v", err)
		}
		close(done)
	}()
	var wg sync.WaitGroup
	wg.Add(1)
	go func() {
		fakes[0].Send(1, []byte("foobar"))
		fakes[0].Send(0, []byte("foobaz"))
		if got, want := fakes[0].RecvFrom(0), (&Message{Source: 0, Target: 0, Message: []byte("foobaz")}); !equivalentMessages(got, want) {
			t.Errorf("unexpected message received: got=%+v, want=%+v", got, want)
		}
		if got, want := fakes[0].Recv(), (&Message{Source: 1, Target: 0, Message: []byte("barbaz")}); !equivalentMessages(got, want) {
			t.Errorf("unexpected message received: got=%+v, want=%+v", got, want)
		}
		fakes[0].Close()
		wg.Done()
	}()
	wg.Add(1)
	go func() {
		fakes[1].Send(0, []byte("barbaz"))
		fakes[1].Recv()
		fakes[1].Close()
		wg.Done()
	}()
	wg.Wait()
	<-done
}

// TODO: test the timestamp-ordering mechanism
// TODO: test deadlock detection (check for false positives too)
// TODO: test remaining messages detection
