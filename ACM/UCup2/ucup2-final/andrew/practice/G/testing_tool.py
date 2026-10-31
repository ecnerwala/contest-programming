#!/usr/bin/env python3

"""
Testing tool for the Guess the Polygon problem

Usage:

  python3 testing_tool.py -f inputfile <program invocation>


Use the -f parameter to specify the input file, e.g. 1.in.

Format of the input file:
- The first line should contain a single integer T (1 <= T <= 1000),
indicating the number of test cases. For each test case:
- The first line should contain a single integer n (3 <= n <= 1000),
indicating the number of vertices of the polygon.
- The next n lines should contain two integers x and y (0 <= x, y <= 1000)
each, indicating the coordinates (x, y) of the vertices of the polygon.

e.g.:
2
4
0 0
1 1
3 0
1 3
3
0 0
1000 999
999 1000

Note that the tool will NOT fully validate the input file, so make sure the
input file you provide is valid.
The way the vertices are shuffled is NOT guaranteed to be the same as the one
used in the actual test.


You can compile and run your solution as follows:

C++:
  g++ solution.cpp
  python3 testing_tool.py -f 1.in ./a.out

Python:
  python3 testing_tool.py -f 1.in python3 ./solution.py

Java:
  javac solution.java
  python3 testing_tool.py -f 1.in java solution

Kotlin:
  kotlinc solution.kt
  python3 testing_tool.py -f 1.in kotlin solutionKt


The tool is provided as-is, and you should feel free to make
whatever alterations or augmentations you like to it.

The tool attempts to detect and report common errors, but it is not guaranteed
that a program that passes the testing tool will be accepted.
"""

from __future__ import annotations

import argparse
import random
import subprocess
import traceback
import math


from fractions import Fraction
from itertools import count
from typing import cast, Literal, NamedTuple

Query = tuple[int, int]

class Point(NamedTuple):
    x: int
    y: int

    def cross(self, other: Point) -> int:
        return self.x * other.y - self.y * other.x

    def dot(self, other: Point) -> int:
        return self.x * other.x + self.y * other.y

    def __add__(self, other: Point) -> Point:
        return Point(self.x + other.x, self.y + other.y)

    def __sub__(self, other: Point) -> Point:
        return Point(self.x - other.x, self.y - other.y)

    def __mul__(self, other: int) -> Point:
        return Point(self.x * other, self.y * other)

random.seed('quq')

parser = argparse.ArgumentParser(description="Testing tool for the Guess the Polygon problem")
parser.add_argument(
    "-f",
    dest="inputfile",
    metavar="inputfile",
    default=None,
    type=argparse.FileType("r"),
    required=True,
    help="The input file to use.",
)
parser.add_argument("program", nargs="+", help="Invocation of your solution")

args = parser.parse_args()

samples = []
with args.inputfile as f:
    T = int(f.readline())
    assert 1 <= T <= 1000, "T is out of range, expected 1 <= T <= 1000."
    for _ in range(T):
        n = int(f.readline())
        assert 3 <= n <= 1000, "n is out of range, expected 3 <= n <= 1000."
        sample = []
        for _ in range(n):
            x, y = map(int, f.readline().split())
            assert 0 <= x <= 1000, "x is out of range, expected 0 <= x <= 1000."
            assert 0 <= y <= 1000, "y is out of range, expected 0 <= y <= 1000."
            sample.append((x, y))
        samples.append(sample)


with subprocess.Popen(
    " ".join(args.program),
    shell=True,
    stdout=subprocess.PIPE,
    stdin=subprocess.PIPE,
    universal_newlines=True,
) as p:
    if p.stdout is None or p.stdin is None:
        raise RuntimeError("Failed to open program's pipes.")
 
    def write(line: str) -> None:
        assert p.poll() is None, "Program terminated early"
        print(f"> {line}", flush=True)
        p.stdin.write(f"{line}\n")
        p.stdin.flush()

    def read() -> str:
        line = p.stdout.readline().strip()
        assert line != "", "Read empty line or closed output pipe. Make sure that your program started successfully."
        print(f"< {line}", flush=True)
        return line

    def read_query(t: int, n: int) -> tuple[Literal[True], Query] | tuple[Literal[False], list[Point]]:
        line = read().strip().split()
        if len(line) == 0:
            assert False, "Unexpected EOF."
        if line[0] == "!":
            ans = tuple(map(int, line[1:]))
            assert len(ans) == 2, "Expected two integers."
            u, v = ans
            assert u >= 0, "u is out of range, expected u >= 0."
            assert v >= 1, "v is out of range, expected v >= 1."
            assert math.gcd(u, v) == 1, "Invalid query, expected gcd(u, v) = 1."
            return False, (u, v)
        assert line[0] == "?", f"Unexpected option {line[0]}, expected '?' or '!'."
        query = tuple(map(int, line[1:]))
        assert len(query) == 2, "Expected two integers."
        p, q = query
        assert 1 <= q <= 1000, "q is out of range, expected 1 <= q <= 1000."
        assert 0 <= p <= 1000 * q, "p / q is out of range, expected 0 <= p / q <= 1000."
        assert t <= n - 2, "Number of queries exceeds the limit."
        return True, (p, q)

    def solve(poly: list[Point], p: int, q: int) -> None:
        x = Fraction(p, q)
        events: list[tuple[Fraction, int]] = []
        sgn = lambda x: bool(x > 0) - bool(x < 0)
        for l, r in zip(poly, poly[1:] + poly[:1]):
            sl = sgn(x - l.x)
            sr = sgn(x - r.x)
            if sl != sr:
                y = Fraction(r.y - l.y, r.x - l.x) * (x - l.x) + l.y
                events.append((y, sr - sl))
        events.sort()
        cnt, las = 0, 0
        can: list[Fraction] = []
        for y, t in events:
            if cnt == 0:
                las = y
            cnt += t
            if cnt == 0 and y != las:
                can.append(y - las)
        res = sum(can)
        write(f"{res.numerator} {res.denominator}")

    def go(poly: list[Point]) -> None:
        n = len(poly)
        for t in count(1):
            is_query, ans = read_query(t, n)
            if is_query:
                solve(poly, *cast(Query, ans))
            else:
                u, v = ans
                res = sum(Fraction(poly[i - 1].cross(poly[i]), 2) for i in range(n))
                assert res == Fraction(u, v), f"Wrong answer, expected {res.numerator} / {res.denominator}."
                return

    try:
        write(len(samples))
        for num, sample in enumerate(samples, 1):
            poly = list(map(Point._make, sample))
            write(len(poly))
            spoly = poly.copy()
            random.shuffle(spoly)
            for point in spoly:
                write(f"{point.x} {point.y}")
            go(poly)
            print(f"Test case #{num} passed.", flush=True)

        assert p.stdout.readline() == "", "Your submission printed extra data after finishing."
        assert p.wait() == 0, "Your submission did not exit cleanly after finishing."

    except AssertionError as e:
        print()
        print(f"Wrong answer: {e}",)
        print()
        try:
            p.wait(timeout=2)
        except subprocess.TimeoutExpired:
            print("Killing your submission after 2 second timeout.")
            p.kill()

    except Exception as e:
        print()
        traceback.print_exc()
        print()
        try:
            p.wait(timeout=2)
        except subprocess.TimeoutExpired:
            print("Killing your submission after 2 second timeout.")
            p.kill()

    finally:
        print(f"Exit code: {p.wait()}", flush=True)

