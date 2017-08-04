package main

import (
	"bytes"
	"io"
	"io/ioutil"
	"sync"
	"testing"
)

// infiniteReader generates an infinite and deterministic stream of bytes
type infiniteReader int

func (ir *infiniteReader) Read(buf []byte) (int, error) {
	for i := range buf {
		// We want the cycle to be long to detect wrong read offsets more surely.
		buf[i] = byte(*ir ^ (*ir >> 8))
		*ir++
	}
	return len(buf), nil
}

func testReader() io.Reader {
	const N = 100 * 1024 // more than 2*32k, so that io.Copy will do 3 reads from it
	var ir infiniteReader
	return io.LimitReader(&ir, N)
}

func expectEqual(t *testing.T, got, want []byte) {
	if bytes.Equal(got, want) {
		return
	}
	size := len(want)
	if len(got) < size {
		size = len(got)
	}
	for i := 0; i < size; i++ {
		if want[i] != got[i] {
			t.Errorf("value read differs from expected on byte %d: got=%d, want=%d", i, got[i], want[i])
			return
		}
	}
	if len(got) != len(want) {
		t.Errorf("value read is %d bytes long, where %d was expected", len(got), len(want))
	}
}

func TestFilePipeSimple(t *testing.T) {
	fp, err := NewFilePipe()
	if err != nil {
		t.Fatalf("Failed to create a filepipe: %v", err)
	}
	fpr := fp.Reader()
	_, err = io.Copy(fp, testReader())
	if err != nil {
		t.Errorf("Failed to write to a filepipe: %v", err)
	}
	err = fp.Close()
	if err != nil {
		t.Errorf("Failed to close a filepipe: %v", err)
	}
	got, err := ioutil.ReadAll(fpr)
	if err != nil {
		t.Fatalf("Failed to read from a filepipe reader: %v", err)
	}
	want, err := ioutil.ReadAll(testReader())
	if err != nil {
		t.Fatalf("error reading from a testReader: %v", err)
	}
	expectEqual(t, got, want)
}

func TestFilePipeConcurrent(t *testing.T) {
	want, err := ioutil.ReadAll(testReader())
	if err != nil {
		t.Fatalf("error reading from a testReader: %v", err)
	}
	fp, err := NewFilePipe()
	if err != nil {
		t.Fatalf("Failed to create a filepipe: %v", err)
	}
	var wg sync.WaitGroup
	const P = 10
	for i := 0; i < P; i++ {
		wg.Add(1)
		fpr := fp.Reader()
		go func(fpr io.Reader) {
			buf, err := ioutil.ReadAll(fpr)
			if err != nil {
				t.Fatalf("Failed to read from a filepipe reader: %v", err)
			}
			expectEqual(t, buf, want)
			wg.Done()
		}(fpr)
	}
	_, err = io.Copy(fp, testReader())
	if err != nil {
		t.Errorf("Failed to write to a filepipe: %v", err)
	}
	err = fp.Close()
	if err != nil {
		t.Errorf("Failed to close a filepipe: %v", err)
	}
	wg.Wait()
}

func TestFilePipeRelease(t *testing.T) {
	fp, err := NewFilePipe()
	if err != nil {
		t.Fatalf("error creating a filepipe: %v", err)
	}
	fpr := fp.Reader()
	err = fp.Release()
	if err != nil {
		t.Fatalf("error releasing a filepipe: %v", err)
	}
	var buf [10]byte
	_, err = fpr.Read(buf[:])
	if err == nil {
		t.Errorf("no error when reading from a destroyed filepipe")
	}
	err = fp.Release()
	if err != nil {
		t.Fatalf("error releasing a filepipe for the second time: %v", err)
	}
}

func TestFilePipeClose(t *testing.T) {
	fp, err := NewFilePipe()
	if err != nil {
		t.Fatalf("error creating a filepipe: %v", err)
	}
	err = fp.Close()
	if err != nil {
		t.Fatalf("error closing a filepipe: %v", err)
	}
	_, err = fp.Write([]byte("foo"))
	if err == nil {
		t.Errorf("no error when writing to a closed filepipe")
	}
}
