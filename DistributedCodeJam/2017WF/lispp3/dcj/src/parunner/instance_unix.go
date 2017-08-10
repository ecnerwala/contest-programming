// +build darwin dragonfly freebsd linux netbsd openbsd solaris

package main

import (
	"os"
	"os/exec"
)

func startInstance(cmd *exec.Cmd, r *os.File, w *os.File) error {
	cmd.ExtraFiles = []*os.File{r, w}
	return cmd.Start()
}
