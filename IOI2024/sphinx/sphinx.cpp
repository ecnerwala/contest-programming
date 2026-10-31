#include "sphinx.h"

#include <bits/stdc++.h>

std::vector<int> find_colours(int N, std::vector<int> X, std::vector<int> Y) {
	int M = int(X.size());
	assert(M == int(Y.size()));
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		adj[X[e]].push_back(Y[e]);
		adj[Y[e]].push_back(X[e]);
	}

	// Count the number of colored components
	auto count_comps = [&](std::vector<int>& C) -> int {
		int res = 0;
		std::vector<int> q; q.reserve(N);
		std::vector<bool> vis(N, false);
		for (int st = 0; st < N; st++) {
			if (!vis[st] && C[st] != -1) {
				res++;
				vis[st] = true;
				q.push_back(st);
				for (int idx = 0; idx < int(q.size()); idx++) {
					int cur = q[idx];
					for (int nxt : adj[cur]) {
						if (!vis[nxt] && C[nxt] != -1) {
							vis[nxt] = true;
							q.push_back(nxt);
						}
					}
				}
				q.clear();
			}
		}
		return res;
	};

	std::vector<int> group(N);
	std::iota(group.begin(), group.end(), 0);

	// First, find all edges which are equal
	for (int i = 0; i < N; i++) {
		std::vector<int> edges; edges.reserve(N);
		std::vector<bool> group_used(N);
		for (auto x : adj[i]) {
			if (x > i) continue;
			int g = group[x];
			if (!group_used[g]) {
				group_used[g] = true;
				edges.push_back(x);
			}
		}

		std::vector<int> C(N, N);
		C[i] = -1;
		for (int x : edges) C[x] = -1;
		int tgt = 1 + int(edges.size()) + count_comps(C);
		int num_edges = tgt - perform_experiment(C);

		for (; num_edges > 0; num_edges--) {
			// binary search over edges
			int mi = 0;
			int ma = int(edges.size());
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				std::fill(C.begin(), C.end(), N);
				C[i] = -1;
				for (int j = md; j < ma; j++) {
					C[edges[j]] = -1;
				}
				if (1 + (ma - md) + count_comps(C) - perform_experiment(C) > 0) {
					mi = md;
				} else {
					ma = md;
				}
			}
			int x = edges[mi];
			int g = group[x];
			for (int j = 0; j < N; j++) {
				if (group[j] == g) group[j] = i;
			}
			edges.resize(mi);
		}
	}

	// Now we only have non-equal groups

	int num_groups = 0;
	for (int i = 0; i < N; i++) {
		if (group[i] == i) num_groups++;
	}

	if (num_groups == 1) {
		// If they're all the same group, just exclude 1
		for (int c = 0; c < N; c++) {
			std::vector<int> C(N, c);
			C[0] = -1;
			if (perform_experiment(C) == 1) {
				return std::vector<int>(N, c);
			}
		}
		assert(false);
	}

	std::vector<std::vector<int>> group_adj(N);
	for (int e = 0; e < M; e++) {
		int a = group[X[e]];
		int b = group[Y[e]];
		if (a != b) {
			group_adj[a].push_back(b);
			group_adj[b].push_back(a);
		}
	}

	std::vector<int> side(N, -1);
	{
		std::vector<int> q; q.reserve(num_groups);
		side[N-1] = 0;
		q.push_back(N-1);
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			for (int nxt : group_adj[cur]) {
				if (side[nxt] == -1) {
					side[nxt] = !side[cur];
					q.push_back(nxt);
				}
			}
		}
		assert(int(q.size()) == num_groups);
	}

	std::vector<int> color(N, -1);

	for (int z = 0; z < 2; z++) {
		for (int c = 0; c < N; c++) {
			std::vector<int> in_groups; in_groups.reserve(num_groups);
			std::vector<int> C(N);
			for (int i = 0; i < N; i++) {
				if (side[group[i]] == z) {
					if (group[i] == i) {
						in_groups.push_back(i);
					}
					C[i] = -1;
				} else {
					C[i] = c;
				}
			}
			int whole_experiment = perform_experiment(C);
			int tgt = int(in_groups.size()) + count_comps(C);
			while (whole_experiment < tgt) {
				assert(!in_groups.empty());
				int mi = 0;
				int ma = int(in_groups.size());
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					std::fill(C.begin(), C.end(), c);
					for (int j = md; j < ma; j++) C[in_groups[j]] = -1;
					for (int i = 0; i < N; i++) {
						C[i] = C[group[i]];
					}
					if ((ma - md) + count_comps(C) > perform_experiment(C)) {
						mi = md;
					} else {
						ma = md;
					}
				}
				int g = in_groups[mi];
				color[g] = c;
				in_groups.resize(mi);

				// reset C for our loop condition
				tgt = 0;
				for (int i = 0; i < N; i++) {
					if (side[group[i]] == z && color[group[i]] != c) {
						if (group[i] == i) tgt++;
						C[i] = -1;
					} else {
						C[i] = c;
					}
				}
				tgt += count_comps(C);
			}
		}
	}

	for (int i = 0; i < N; i++) {
		color[i] = color[group[i]];
		assert(color[i] != -1);
	}

	return color;
}
