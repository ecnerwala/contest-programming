import z3
from z3 import *

def solve(t):
	N,M = map(int,input().split())
	C = list(map(int,input().split()))
	adj = [[] for i in range(N)]
	for e in range(M):
		a,b = map(int,input().split())
		a,b = a-1, b-1
		adj[a].append(b)
		adj[b].append(a)

	tot = 0

	for src in range(N):
		for dst in range(src+1, N):
			par = [-1] * N
			bfs = [src]
			vis = [0] * N
			idx = 0
			while idx < len(bfs):
				v = bfs[idx]
				idx += 1
				for w in adj[v]:
					if vis[w]:
						continue
					vis[w] = 1
					bfs.append(w)
					par[w] = v
			path = [dst]
			while path[-1] != src:
				path.append(par[path[-1]])
			path = path[::-1]
			path_idx = [-1] * N
			for i, v in enumerate(path):
				path_idx[v] = i
			
			on_cycle = [0] * N

			cycles = []
			for i, cycle_start in enumerate(path[:-1]):
				bfs = [cycle_start]
				new_par = [-1] * N
				vis = [0] * N
				idx = 0
				found_cycles = []
				while idx < len(bfs):
					v = bfs[idx]
					idx += 1
					for w in adj[v]:
						if vis[w]:
							continue
						if path_idx[v] >= 0 and path_idx[w] >= 0 and (w == par[v] or v == par[w]):
							continue
						if path_idx[w] > path_idx[cycle_start]:
							vis[w] = 1
							new_par[w] = v
							found_cycles.append(w)
							continue
						if path_idx[w] >= 0:
							continue
						vis[w] = 1
						bfs.append(w)
						new_par[w] = v
				found_cycles = list(set(found_cycles))
				assert len(found_cycles) <= 1
				for found_cycle in found_cycles:
					old_verts = [found_cycle]
					while old_verts[-1] != cycle_start:
						old_verts.append(par[old_verts[-1]])
					new_verts = [found_cycle]
					while new_verts[-1] != cycle_start:
						new_verts.append(new_par[new_verts[-1]])
					for x in old_verts[1:-1]:
						assert on_cycle[x] == 0
						on_cycle[x] = 1
					cycles.append((old_verts[1:-1], new_verts[1:-1]))
			vars = [Bool('x' + str(i)) for i in range(len(cycles))]
			model = z3.Solver()

			# print(cycles)

			ans = 0
			for mex in range(N):
				cond = False
				for v in path:
					if not on_cycle[v] and C[v] == mex:
						cond = True
				for i, (l, r) in enumerate(cycles):
					if any(mex == C[x] for x in l):
						cond = Or(cond, Not(vars[i]))
					if any(mex == C[x] for x in r):
						cond = Or(cond, vars[i])
				model.add(cond == True)
				if model.check() == sat:
					ans += 1
				else:
					break
			print(src, dst, ans)
			tot += ans
	print(f"Case #{t}: ", end='')
	print(tot)
T = int(input())
for t in range(1,T+1):
	solve(t)
