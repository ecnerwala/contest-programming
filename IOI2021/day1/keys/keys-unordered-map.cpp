#include "keys.h"

#include <bits/stdc++.h>

namespace ecnerwala {

struct par_compressor {
	mutable std::vector<int> par;
	par_compressor(int N) : par(N, -1) {}
	int get_par(int a) const {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	}
	void set_par(int a, int b) {
		assert(par[a] < 0 && par[b] < 0);
		assert(a != b);
		par[b] += par[a];
		par[a] = b;
	}
	int cc_sz(int a) const {
		assert(par[a] < 0);
		return -par[a];
	}
};

}

std::vector<int> find_reachable(std::vector<int> R, std::vector<int> U, std::vector<int> V, std::vector<int> C) {
	using namespace ecnerwala;

	int N = int(R.size());
	int M = int(U.size());
	assert(M == int(V.size()));
	assert(M == int(C.size()));

	U.insert(U.end(), V.begin(), V.end());
	V = {};

	std::vector<int> edge_nxt(2*M, -1);

	struct linked_list {
		int st, en;
		linked_list() : st(-1), en(-1) {}
		explicit linked_list(int a) : st(a), en(a) {}
	};
	auto concat = [&](linked_list& a, linked_list& b) {
		if (b.st == -1) return;
		if (a.st == -1) {
			a = b;
			b.st = b.en = -1;
			return;
		}

		edge_nxt[a.en] = b.st;
		a.en = b.en;
		b.st = b.en = -1;
	};

	using edge_set = std::unordered_map<int, linked_list>;

	std::vector<linked_list> unlocked_edges(N);
	std::vector<edge_set> locked_edges(N);
	for (int i = 0; i < N; i++) {
		locked_edges[i][R[i]] = linked_list(-2);
	}
	for (int e = 0, o = M; e < 2*M; e++, o++) {
		if (o == 2*M) o = 0;
		int u = U[o];
		int c = C[std::min(e, o)];
		linked_list new_edge(e);
		if (c == R[u]) concat(unlocked_edges[u], new_edge);
		else concat(locked_edges[u].insert({c, linked_list()}).first->second, new_edge);
	}

	par_compressor compressed_node_par(N);
	std::vector<int> forest_par(N, -1);
	par_compressor forest_root(N);

	int best_sz = N+1;

	for (int st = 0; st < N; st++) {
		while (true) {
			int e = unlocked_edges[st].st;
			if (e == -1) {
				best_sz = std::min(best_sz, compressed_node_par.cc_sz(st));
				break;
			}
			unlocked_edges[st].st = edge_nxt[e];
			if (edge_nxt[e] == -1) unlocked_edges[st].en = -1;
			edge_nxt[e] = -1;

			int dest = compressed_node_par.get_par(U[e]);
			int dest_tree = forest_root.get_par(dest);
			if (dest_tree == st) {
				// compress it
				while (dest != st) {
					// merge dest into st
					concat(unlocked_edges[st], unlocked_edges[dest]);
					if (locked_edges[dest].size() > locked_edges[st].size()) {
						std::swap(locked_edges[st], locked_edges[dest]);
					}
					for (auto& [c, dv] : locked_edges[dest]) {
						auto& sv = locked_edges[st].insert({c, linked_list()}).first->second;
						if (dv.st == -2) std::swap(sv, dv);
						if (sv.st == -2) {
							if (dv.st != -2) concat(unlocked_edges[st], dv);
						} else {
							concat(sv, dv);
						}
					}
					locked_edges[dest] = {};

					compressed_node_par.set_par(dest, st);
					dest = compressed_node_par.get_par(forest_par[dest]);
				}
			} else {
				// Link ourself up to dest and keep going
				forest_par[st] = dest;
				forest_root.set_par(st, dest_tree);
				break;
			}
		}
	}

	std::vector<int> ans(N, false);
	for (int i = 0; i < N; i++) {
		int n = compressed_node_par.get_par(i);
		if (forest_par[n] != -1) continue;
		if (compressed_node_par.cc_sz(n) == best_sz) ans[i] = true;
	}

	return ans;
}
