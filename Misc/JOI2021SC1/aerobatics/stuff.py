import numpy as np
import matplotlib.pyplot as plt
import glob

for n in [1,2,3,4,5,6]:
#for n in [6]:
    casename = f"input_0{n}"
    with open(f'{casename}.in') as f:
        (N, z0), *pts = [tuple(map(int, l.strip().split())) for l in f]
    print(N, z0)
    best_solution_file = max(glob.glob(f'{casename}*.out'))
    #best_solution_file = 'input_06-manual.out'
    print(best_solution_file)
    with open(best_solution_file) as f:
        ans = [int(l.strip())-1 for l in f]
    plt.axes().set_aspect('equal')
    plt.plot([pts[i][0] for i in ans], [pts[i][1] for i in ans], 'b-')
    for i in range(N):
        plt.annotate(i+1, (pts[i]))
    plt.show()
