import matplotlib.pyplot as plt
import math

def small():
    data = []
    with open("small.csv") as f:
        data = [[int(x) for x in l.rstrip()] for l in f]
    for i, row in enumerate(data):
        if i % 5 == 0:
            print(''.join(str(v) for j, v in enumerate(row) if j % 5 == 0))

def large():
    data = []
    with open("large.csv") as f:
        data = [[int(x) for x in l.rstrip().split(',')] for l in f]
    #print(data)

    for i, row in enumerate(data):
        for j, v in enumerate(row):
            hrs = v / 3600 / 12
            mins = v / 60 / 60
            secs = v / 60
            r = 0.5
            j -= i
            plt.plot([j, j + r * math.sin(hrs * math.tau)], [-i, -i + r * math.cos(hrs * math.tau)], color='black')
            plt.plot([j, j + r * math.sin(mins * math.tau)], [-i, -i + r * math.cos(mins * math.tau)], color='black')
            #plt.plot([j, j + math.sin(secs * math.tau)], [-i, -i - math.cos(secs * math.tau)], color='black')
    plt.gca().set_aspect('equal')
    plt.show()

small()
#large()
