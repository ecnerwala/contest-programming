package main

import (
	"bytes"
	"io/ioutil"
	"os"
	"os/exec"
	"strings"
	"testing"
)

func TestInstances(t *testing.T) {
	if _, err := os.Stat(testerPath); err != nil {
		t.Fatalf("can't find tester binary: %v", err)
	}
	type testcase struct {
		name             string
		inputs           []string
		expectedOutputs  []string // only used if len(expectedFails) == 0
		expectedFails    []int    // if empty, we expect success; otherwise we expect failure from one of the numerated instances
		expectedDeadlock bool
	}
	testcases := []*testcase{
		{"no comm", []string{"", "", "", ""}, []string{"0 4\n", "1 4\n", "2 4\n", "3 4\n"}, nil, false},
		{"fail", []string{"", "Q 1", "Q 2"}, nil, []int{1, 2}, false},
		{"send with no recv", []string{"", "Safoo\n"}, []string{"0 2\n", "1 2\n"}, nil, false},
		{"send with recv", []string{"Rb\n", "Safoo\n"}, []string{"0 2\n1 3 foo\n", "1 2\n"}, nil, false},
		{"send with recvany", []string{"R*\n", "Safoo\n"}, []string{"0 2\n1 3 foo\n", "1 2\n"}, nil, false},
		{"deadlock", []string{"Scfoo\nR*\n", "R*\n", ""}, nil, nil, true},
		{"fail and hang", []string{"H\n", "Q 1\n"}, nil, []int{1}, false},
		{"fail and hanging recv", []string{"R*\n", "Q 1\n"}, nil, []int{1}, false},
	}
	for _, tc := range testcases {
		outputs := make([]bytes.Buffer, len(tc.inputs))
		cmds := make([]*exec.Cmd, len(tc.inputs))
		for i, input := range tc.inputs {
			cmds[i] = exec.Command(testerPath)
			cmds[i].Stdin = strings.NewReader(input)
			cmds[i].Stdout = &outputs[i]
		}
		_, err := RunInstances(cmds, ioutil.Discard)
		if _, ok := err.(ErrRemainingMessages); ok {
			err = nil
		}
		if err != nil {
			switch err := err.(type) {
			case InstanceError:
				ok := false
				for _, i := range tc.expectedFails {
					if err.ID == i {
						ok = true
					}
				}
				if !ok {
					t.Errorf("test %s: unexpected error from RunInstances: %v", tc.name, err)
				}
			case ErrDeadlock:
				if !tc.expectedDeadlock {
					t.Errorf("test %s: unexpected deadlock", tc.name)
				}
			default:
				t.Errorf("test %s: unexpected error from RunInstances: %v", tc.name, err)
			}
			continue
		}
		if len(tc.expectedFails) != 0 || tc.expectedDeadlock {
			t.Errorf("test %s: unexpected success of RunInstances", tc.name)
			continue
		}
		for i, want := range tc.expectedOutputs {
			got := strings.Replace(outputs[i].String(), "\r\n", "\n", -1)
			if got != want {
				t.Errorf("test %s: wrong output from instance %d: got=%q, want=%q", tc.name, i, got, want)
			}
		}
	}
}

func TestInstancesStartError(t *testing.T) {
	cmds := []*exec.Cmd{exec.Command("/does/not/exist")}
	_, err := RunInstances(cmds, ioutil.Discard)
	if err == nil {
		t.Errorf("expected an error when trying to run a nonexistent binary")
	}
}

// TODO: check what happens when we send/recv message to/from an instance that doesn't exist
// TODO: check what happens when an instance claims that its CPU time goes backward
