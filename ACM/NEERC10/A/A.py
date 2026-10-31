#/usr/bin/env python3

data = []
with open("alignment.in") as f:
    for line in f:
        data.append(line.split())
cols = max(map(len, data))
widths = [max(len(d[c]) for d in data if c < len(d)) for c in range(cols)]
outputs = [
    ' '.join(s + ' '*(w-len(s)) for s,w in zip(d, widths)).rstrip()
    for d in data
]
with open("alignment.out", 'w') as f:
    f.write('\n'.join(outputs))
