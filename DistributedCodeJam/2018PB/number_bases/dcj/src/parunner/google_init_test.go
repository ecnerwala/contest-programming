// This file contains google3-specific code to make the third party tests work
// with blaze.

package main

import (
	"fmt"
	"os"
	"path/filepath"
)

func init() {
	dir := filepath.Join(os.Getenv("TEST_SRCDIR"), "google3/third_party/golang/parunner")
	if err := os.Chdir(dir); err != nil {
		panic(fmt.Sprintf("os.Chdir(%q): %v", dir, err))
	}
}
