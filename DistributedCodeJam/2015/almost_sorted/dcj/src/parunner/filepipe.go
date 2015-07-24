package main

import (
	"errors"
	"io"
	"io/ioutil"
	"os"
	"runtime"
	"sync"
)

// FilePipe is a tailable buffer backed by a file.
type FilePipe struct {
	f *os.File

	mu      sync.Mutex
	cond    sync.Cond
	size    int64
	closing bool
}

// Create a new FilePipe backed by a temporary file.
func NewFilePipe() (*FilePipe, error) {
	// On some OSes we could remove the file immediately.
	f, err := ioutil.TempFile("", "filepipe")
	if err != nil {
		return nil, err
	}
	fp := &FilePipe{f: f}
	fp.cond.L = &fp.mu
	runtime.SetFinalizer(fp, (*FilePipe).Release)
	return fp, nil
}

// Release releases the resources associated with the filepipe. In particular,
// it removes the backing file. No readers should be used concurrently with a
// call to Release, nor after a call to Release. Release is idempotent.
func (fp *FilePipe) Release() error {
	fp.Close()
	if fp.f == nil {
		return nil
	}
	filename := fp.f.Name()
	err := fp.f.Close()
	if err1 := os.Remove(filename); err == nil {
		err = err1
	}
	fp.f = nil
	return err
}

// Reader creates a new reader that starts reading from the beginning of
// the filepipe's contents and blocks at the end until the filepipe is closed.
func (fp *FilePipe) Reader() io.Reader {
	return &filePipeReader{fp: fp, pos: 0}
}

func (fp *FilePipe) Write(buf []byte) (int, error) {
	n, err := fp.f.Write(buf)
	fp.mu.Lock()
	defer fp.mu.Unlock()
	if fp.closing {
		return 0, errors.New("write to a closed filepipe")
	}
	fp.size += int64(n)
	fp.cond.Broadcast()
	return n, err
}

// Close finalizes the filepipe's contents. Once Close is called, all readers
// that read up to the end of the contents will return io.EOF instead of waiting
// for more data. Close is idempotent.
func (fp *FilePipe) Close() error {
	fp.mu.Lock()
	fp.closing = true
	fp.cond.Broadcast()
	fp.mu.Unlock()
	return nil
}

type filePipeReader struct {
	fp  *FilePipe
	pos int64
}

func (fpr *filePipeReader) Read(buf []byte) (int, error) {
	if fpr.fp.f == nil {
		return 0, errors.New("filepipe already had its resources released")
	}
	for {
		n, err := fpr.fp.f.ReadAt(buf, fpr.pos)
		fpr.pos += int64(n)
		if err == io.EOF {
			err = nil
		}
		if err != nil || n > 0 {
			return n, err
		}
		fpr.fp.mu.Lock()
		for fpr.pos >= fpr.fp.size && !fpr.fp.closing {
			fpr.fp.cond.Wait()
		}
		eof := fpr.pos >= fpr.fp.size && fpr.fp.closing
		fpr.fp.mu.Unlock()
		if eof {
			return 0, io.EOF
		}
	}
}
