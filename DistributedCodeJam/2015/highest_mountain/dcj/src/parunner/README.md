parunner
========

Single-machine runner for [distributed](http://potyczki.mimuw.edu.pl/l/zadania_rozproszone/) [Potyczki Algorytmiczne](http://potyczki.mimuw.edu.pl/) problems ([a](https://sio2.mimuw.edu.pl/pa/c/pa-2014-1/p/mak/) [few](https://sio2.mimuw.edu.pl/pa/c/pa-2014-1/p/kol/) [examples](https://sio2.mimuw.edu.pl/pa/c/pa-2014-1/p/sek/)).

[![Build Status](https://drone.io/github.com/robryk/parunner/status.png)](https://drone.io/github.com/robryk/parunner/latest) [![GoDoc](https://godoc.org/github.com/robryk/parunner?status.png)](https://godoc.org/github.com/robryk/parunner)

Usage
-----

In order to run a program that uses [raw zeus interface](https://github.com/robryk/parunner/blob/master/zeus/zeus.h), you need to link it with [zeus/zeus_local.c](https://github.com/robryk/parunner/blob/master/zeus/zeus_local.c) instead of any other implementation of zeus_local. You can then run the program as follows:

    $ parunner -n=number_of_instances path/to/program

There is an [example](https://github.com/robryk/parunner/blob/master/zeus/example.c) provided. In order to run it, you should:

1. Compile it: `make -C zeus example`
2. Obtain a binary of parunner. If you have a Go toolchain installed, you can compile it by doing `go get github.com/robryk/parunner`. The binary will then be built and written to `$GOPATH/bin/parunner`. There is also a compiled binary for [linux-amd64](https://drone.io/github.com/robryk/parunner/files/parunner) available.
3. Run `parunner -n=3 -trace_comm -stdout=tagged zeus/example`. The output should look like this:
```
robryk@sharya-rana ~/g/s/g/r/parunner> parunner -n=3 -trace_comm -stdout=tagged zeus/example
STDOUT 0: Nodeow jest 3, a ja mam numer 0.
STDOUT 0: Wysylam wiadomosc do 1.
STDOUT 1: Nodeow jest 3, a ja mam numer 1.
STDOUT 1: Wysylam wiadomosc do 2.
STDOUT 1: Odbieram wiadomosc od 0.
STDOUT 2: Nodeow jest 3, a ja mam numer 2.
STDOUT 2: Odbieram wiadomosc od 1.
COMM: instancja  1:instancja 0 wysyła do mnie wiadomość (13 bajtów) [0]
COMM: instancja  2:instancja 1 wysyła do mnie wiadomość (13 bajtów) [0]
COMM: instancja  1:czekam na wiadomość od instancji 0 [0]
COMM: instancja  1:odebrałam wiadomość od instancji 0 (13 bajtów)
STDOUT 1: Odebralem: Hello from 0!
COMM: instancja  2:czekam na wiadomość od instancji 1 [0]
COMM: instancja  2:odebrałam wiadomość od instancji 1 (13 bajtów)
STDOUT 2: Odebralem: Hello from 1!
Czas trwania: 0 (najdłużej działająca instancja: 2)
```

For more information on parunner's usage invoke it with no arguments.
