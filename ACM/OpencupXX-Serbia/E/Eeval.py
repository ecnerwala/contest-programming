import sys
lines = list(sys.stdin)
print(len(lines[2:]))
for l in lines[2:]:
	print(eval(l))
