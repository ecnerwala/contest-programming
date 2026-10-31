#!/usr/bin/python3
import argparse
from pathlib import Path
import glob
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument("bin")
parser.add_argument("tests")
parser.add_argument('--run-all', required=False, default=False)

args = parser.parse_args()

bin_path = Path("./") / args.bin
test_path = Path("./") / args.tests

run_all = bool(args.run_all)

single = False
if Path(test_path).is_file():
	single = True
	out_files = [Path(test_path)]
else:
	out_files = list(Path(test_path).glob('**/*.out')) + list(Path(test_path).glob('**/*.ans'))
out_files.sort(key=lambda a: (len(str(a)), a))
used = set()
failed = set()

all_ok = True
for out_file in out_files:
	name = out_file.with_suffix('')
	print(name)

	execute = subprocess.check_call
	if run_all:
		execute = subprocess.call
	ok_run = execute(f"time ./{bin_path} < ./{name}.in > ./{name}.res", shell=True) == 0
	ok_diff = execute(f"diff -Z --strip-trailing-cr ./{name}.res {out_file}", shell=True) == 0

	if ok_run and ok_diff:
		print(out_file, " ok ")
	else:
		print(out_file, " BAD ")
		all_ok = False
		failed.add(str(out_file))

	if single:
		used.add(str(name))
	else:
		used.add(str(name.parent))
print(f"./{bin_path} {test_path} {list(used)} {failed=}")
