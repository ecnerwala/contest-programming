package main

import (
	"bytes"
	"fmt"
	"io"
	"strings"
	"sync"
	"testing"
)

func TestContestStdout(t *testing.T) {
	var buf bytes.Buffer
	cs := &ContestStdout{Output: &buf}

	const N = 10
	var wg sync.WaitGroup
	token := make(chan bool, 1)
	token <- true
	expectedOutput := make(chan string, 1)
	for i := 0; i < N; i++ {
		wg.Add(1)
		go func(i int) {
			defer wg.Done()
			s := fmt.Sprintf("%d\nfoobarbazblah", i)
			w := cs.NewWriter(i)
			_, err := io.Copy(w, strings.NewReader(s))
			if err != nil {
				return
			}
			select {
			case <-token:
			default:
				t.Errorf("instance %d was also able to write", i)
				return
			}
			expectedOutput <- s
		}(i)
	}
	want := <-expectedOutput
	got := buf.String()
	if got != want {
		t.Errorf("wrong output of ContestStdout: got=%q, want=%q", got, want)
	}
}

func TestTagStream(t *testing.T) {
	for _, tc := range []struct {
		input  string
		output string
	}{
		{"foo\n", "PREFIXfoo\n"},
		{"foo", "PREFIXfoo\n"},
		{"foo\n\nbar", "PREFIXfoo\nPREFIX\nPREFIXbar\n"},
	} {
		var buf bytes.Buffer
		if err := TagStream("PREFIX", &buf, strings.NewReader(tc.input)); err != nil {
			t.Errorf("TagStream failed: %v", err)
			continue
		}
		if got := buf.String(); got != tc.output {
			t.Errorf("TagStream returned invalid output: got=%q, want=%q", got, tc.output)
		}
	}
}
