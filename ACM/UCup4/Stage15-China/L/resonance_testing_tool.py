#!/usr/bin/env python3

"""
Local testing tool for Logical Resonance

Disclaimer: This is NOT the same code used to test your solution when it is submitted.
This tool is provided as-is. Its purpose is to help with debugging the interactive
problem and it has no ambitions to extensively test all possibilities that
are allowed by the problem statement. While the tool tries to yield the same
results as the real judging system, this is not guaranteed and the result
may differ if the tested program does not use the correct formatting or
exhibits other incorrect behavior. The tool also *does not* enforce time and
memory limits that are applied to submitted solutions.

Feel free to make whatever alterations or augmentations you like.

The behavior is controlled by an input data file.
The first line contains r and T. This tool expects r=1.
Each test case contains:
  n
  p_1 p_2 ... p_n

Here is an example file:
1 2
4
0 1 1 2
3
0 1 2

The testing tool is run like this::

    pypy3 resonance_testing_tool.py <data_file> <program> <arguments>

where `arguments` are optional arguments to the program to run. The following
show examples for different languages::

    pypy3 resonance_testing_tool.py 1.in ./myprogram
    pypy3 resonance_testing_tool.py 1.in java -cp . MyProgram
    pypy3 resonance_testing_tool.py 1.in pypy3 myprogram.py

The tool logs the complete interaction.
If you do not want that, pass `--quiet` (before the data file name).
"""

from __future__ import annotations

import argparse
import random
import subprocess
import sys
from typing import List, Tuple


verbose = True
process = None
run_id = 0


class WrongAnswer(RuntimeError):
    pass


class EndOfFile(RuntimeError):
    pass


def vprint(*args, **kwargs) -> None:
    if verbose:
        print("< ", end="")
        print(*args, **kwargs)
        sys.stdout.flush()
    print(*args, file=process.stdin, flush=True, **kwargs)


def vreadline(optional: bool = False) -> str:
    line = process.stdout.readline()
    if verbose and line:
        print(">", line.rstrip("\n"))
    if not line and not optional:
        raise EndOfFile()
    return line


def start_process(program: List[str], phase: str) -> subprocess.Popen:
    global run_id, process
    run_id += 1
    if verbose:
        print(f"[run {run_id}] {phase}: {' '.join(program)}")
    process = subprocess.Popen(
        " ".join(program),
        shell=True,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        text=True,
    )
    if process.stdin is None or process.stdout is None:
        raise RuntimeError("Failed to open pipes.")
    return process


def read_input(path: str) -> List[Tuple[int, List[int]]]:
    with open(path, "r", encoding="utf-8") as f:
        tokens = f.read().strip().split()
    if not tokens:
        raise ValueError("Input file is empty.")
    it = iter(tokens)
    r = int(next(it))
    T = int(next(it))
    if r != 1:
        raise ValueError("Input file must start with r=1 for the first run.")
    cases = []
    for _ in range(T):
        n = int(next(it))
        p = [int(next(it)) for _ in range(n)]
        if p[0] != 0:
            raise ValueError("p_1 must be 0.")
        for i in range(1, n):
            if not (1 <= p[i] < i + 1):
                raise ValueError(f"Invalid parent p_{i+1}={p[i]}.")
        cases.append((n, p))
    return cases


def build_run1_input(cases: List[Tuple[int, List[int]]]) -> str:
    lines = [f"1 {len(cases)}"]
    for n, p in cases:
        lines.append(str(n))
        lines.append(" ".join(map(str, p)))
    return "\n".join(lines) + "\n"


def run_program(program: List[str], phase: str, input_data: str) -> List[str]:
    start_process(program, phase)
    for line in input_data.splitlines():
        vprint(line)
    if process.stdin:
        process.stdin.close()

    out_lines = []
    while True:
        line = vreadline(optional=True)
        if line == "":
            break
        out_lines.append(line.rstrip("\n"))

    process.wait(timeout=5)
    if process.returncode != 0:
        raise WrongAnswer(f"{phase} exit code {process.returncode}.")
    return [line for line in out_lines if line.strip() != ""]


def parse_output(lines: List[str], cases: List[Tuple[int, List[int]]], kind: str) -> List[List[int]]:
    if len(lines) != len(cases):
        raise WrongAnswer(f"{kind} output lines {len(lines)} != {len(cases)}.")
    outputs = []
    for (n, _), line in zip(cases, lines):
        parts = line.strip().split()
        if len(parts) != n:
            raise WrongAnswer(f"{kind} output line length {len(parts)} != {n}.")
        vals = list(map(int, parts))
        outputs.append(vals)
    return outputs


def validate_q(n: int, q: List[int]) -> None:
    if q[-1] != 0:
        raise WrongAnswer("q_n must be 0.")
    for i in range(0, n - 1):
        if not (i + 1 < q[i] <= n):
            raise WrongAnswer(f"Invalid q_{i+1}={q[i]}.")


def validate_p(n: int, p: List[int]) -> None:
    if p[0] != 0:
        raise WrongAnswer("p_1 must be 0.")
    for i in range(1, n):
        if not (1 <= p[i] < i + 1):
            raise WrongAnswer(f"Invalid p_{i+1}={p[i]}.")


def main() -> int:
    parser = argparse.ArgumentParser(
        usage="%(prog)s [--quiet] data.in program [args...]"
    )
    parser.add_argument(
        "--quiet", "-q", action="store_true", help="Do not show interactions"
    )
    parser.add_argument("data", help="Input file that controls the behavior of the tool")
    parser.add_argument("program", nargs=argparse.REMAINDER, help="Program to run")
    args = parser.parse_args()

    global verbose
    verbose = not args.quiet

    if not args.program:
        parser.error("Must specify program to run")

    cases = read_input(args.data)
    input_run1 = build_run1_input(cases)
    lines1 = run_program(args.program, "first run", input_run1)
    outputs_q = parse_output(lines1, cases, "first run")

    for (n, p), q in zip(cases, outputs_q):
        validate_q(n, q)

    perm = list(range(len(cases)))
    random.Random(12345).shuffle(perm)

    lines = [f"2 {len(cases)}"]
    for idx in perm:
        n, _ = cases[idx]
        lines.append(str(n))
        lines.append(" ".join(map(str, outputs_q[idx])))
    input_run2 = "\n".join(lines) + "\n"

    lines2 = run_program(args.program, "second run", input_run2)
    outputs_p = parse_output(lines2, [cases[i] for i in perm], "second run")

    for out_p, idx in zip(outputs_p, perm):
        n, orig_p = cases[idx]
        validate_p(n, out_p)
        if out_p != orig_p:
            raise WrongAnswer("Second run output does not match first run input.")

    print("Second run output matches first run input.", file=sys.stderr)
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except WrongAnswer as exc:
        print(f"Wrong answer: {exc}", file=sys.stderr)
        sys.exit(1)
