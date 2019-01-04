from collections import *
from itertools import *
from functools import *
from typing import List

class Unit:
    cnt: int
    health: int
    weak_to: List[str]
    immune_to: List[str]
    attack: int
    attack_type: str
    initiative: int

    @property
    def effective_power(self):
        return self.cnt * self.attack

    def damage_to(self, other):
        if self.attack_type in other.immune_to:
            return 0
        elif self.attack_type in other.weak_to:
            return self.effective_power * 2
        else:
            return self.effective_power

def parse_line(line):
    line = line.strip()
    if '(' in line:
        a, b = line.split('(')
        b, c = b.split(')')
        line = ' '.join([a,c])
        b = b.split('; ')
    else:
        b = []

    a = line.split()
    assert len(a) == 18

    cur = Unit()
    cur.cnt = int(a[0])
    cur.health = int(a[4])
    cur.weak_to = []
    cur.immune_to = []
    for s in b:
        x,y,z = s.split(maxsplit=2)
        assert y == 'to'
        assert x in ['weak', 'immune']
        z = z.split(', ')
        if x == 'weak':
            cur.weak_to = z
        elif x == 'immune':
            cur.immune_to = z
        else: assert False
    cur.attack = int(a[12])
    cur.attack_type = a[13]
    cur.initiative = int(a[17])
    return cur

def simulate(boost=0):
    # TODO: you can actually tie...
    print("starting ", boost)
    units = []
    with open('day24.in') as f:
        for line in f:
            units.append(parse_line(line))
    assert len(units) == 20
    for i in range(0, 10):
        units[i].is_enemy = False
        units[i].index = i + 1
        units[i].attack += boost
    for i in range(10, 20):
        units[i].is_enemy = True
        units[i].index = i-10 + 1
    """
    with open('sample.in') as f:
        for line in f:
            units.append(parse_line(line))
    assert len(units) == 4 
    for i in range(0, 2):
        units[i].is_enemy = False
        units[i].index = i+1
    for i in range(0, 2):
        units[i+2].is_enemy = True
        units[i+2].index = i+1
    """

    while len({u.is_enemy for u in units}) > 1:
        for u in units:
            u.is_targeted = False
        for u in sorted(units, key=lambda u: (u.effective_power, u.initiative), reverse=True):
            u.target = max((v for v in units if u.is_enemy != v.is_enemy and not v.is_targeted), key=lambda v: (u.damage_to(v), v.effective_power, v.initiative), default=None)
            if u.target is not None and u.damage_to(u.target) == 0:
                u.target = None
            if u.target is not None:
                u.target.is_targeted = True
        for u in sorted(units, key=lambda u: u.initiative, reverse=True):
            if not u.cnt: continue
            if u.target is None: continue
            v = u.target
            killedUnits = min(v.cnt, u.damage_to(v) // v.health)
            v.cnt -= killedUnits
        #print("Round done")
        #for u in units: print(u.is_enemy, u.index, u.cnt)
        units = [u for u in units if u.cnt]
    winners = next(u.is_enemy for u in units)
    assert all(u.is_enemy == winners for u in units)
    print(boost, winners, sum(u.cnt for u in units))
    return winners, sum(u.cnt for u in units)

def main():
    print(simulate())
    assert simulate(0)[0] is True
    mi, ma = 0, 1
    while simulate(ma)[0]:
        ma *= 2
        print(ma)
    while ma - mi > 1:
        md = (mi + ma) // 2
        if simulate(md)[0]:
            mi = md
        else:
            ma = md
    print(simulate(ma))

if __name__ == '__main__':
    main()
