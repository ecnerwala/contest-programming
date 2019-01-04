package main

import (
	"fmt"
	"os"
	"os/exec"
	"syscall"
)

func startInstance(cmd *exec.Cmd, r *os.File, w *os.File) error {
	var rHandle syscall.Handle
	var wHandle syscall.Handle
	p, _ := syscall.GetCurrentProcess()
	if err := syscall.DuplicateHandle(p, syscall.Handle(r.Fd()), p, &rHandle, 0, true, syscall.DUPLICATE_SAME_ACCESS); err != nil {
		return err
	}
	defer syscall.CloseHandle(rHandle)
	if err := syscall.DuplicateHandle(p, syscall.Handle(w.Fd()), p, &wHandle, 0, true, syscall.DUPLICATE_SAME_ACCESS); err != nil {
		return err
	}
	defer syscall.CloseHandle(wHandle)

	if cmd.Env == nil {
		cmd.Env = os.Environ()
	}
	cmd.Env = append(cmd.Env, fmt.Sprintf("ZSHANDLE_IN=%d", rHandle), fmt.Sprintf("ZSHANDLE_OUT=%d", wHandle))
	return cmd.Start()
}
