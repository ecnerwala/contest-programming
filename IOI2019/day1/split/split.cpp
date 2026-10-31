#include "split.h"

#include <bits/stdc++.h>
using namespace std;

struct union_find {
	vector<int> par;
	vector<int> sz;
	explicit union_find(int N) : par(N, -1), sz(N, 1) { }

	int get_par(int a) {
		return (par[a] == -1) ? a : (par[a] = get_par(par[a]));
	}

	pair<bool, int> merge(int a, int b) {
		a = get_par(a), b = get_par(b);
		if (a == b) return {false, sz[a]};
		if (sz[a] < sz[b]) swap(a, b);
		par[b] = a;
		sz[a] += sz[b];
		return {true, sz[a]};
	}
};

vector<int> my_find_split(int N, int Asz, int Bsz, int Csz, vector<int> U, vector<int> V) {
	int M = int(U.size());
	vector<pair<int, int>> sizes({{Asz, 1}, {Bsz, 2}, {Csz, 3}});
	sort(sizes.begin(), sizes.end());
	Asz = sizes[0].first, Bsz = sizes[1].first, Csz = sizes[2].first;
	int Alabel = sizes[0].second, Blabel = sizes[1].second, Clabel = sizes[2].second;

	vector<vector<int>> adj(N);
	for (int i = 0; i < M; i++) {
		adj[U[i]].push_back(V[i]);
		adj[V[i]].push_back(U[i]);
	}

	vector<int> par(N, -2);
	vector<int> q; q.reserve(N);
	par[0] = -1;
	q.push_back(0);
	for (int i = 0; i < N; i++) {
		int cur = q[i];
		for (int nxt : adj[cur]) {
			if (par[nxt] != -2) continue;
			par[nxt] = cur;
			q.push_back(nxt);
		}
	}

	vector<int> sz(N);
	for (int i = N-1; i >= 0; i--) {
		int cur = q[i];
		sz[cur]++;
		if (par[cur] != -1) sz[par[cur]] += sz[cur];
	}

	int centroid = -1;
	for (int i = N-1; i >= 0; i--) {
		int cur = q[i];
		if (sz[cur] * 2 >= N) {
			centroid = cur;
			break;
		}
	}
	assert(centroid != -1);

	union_find uf(N);
	for (int i = N-1; i >= 1; i--) {
		int cur = q[i];
		assert(par[cur] != -1);
		if (cur == centroid || par[cur] == centroid) continue;
		uf.merge(cur, par[cur]);
	}

	int Astart = -1;

	for (int i = 0; i < N; i++) {
		if (i == centroid) continue;
		if (uf.sz[uf.get_par(i)] >= Asz) {
			// this is it
			Astart = i;
			goto found_a;
		}
	}
	for (int e = 0; e < M; e++) {
		if (U[e] == centroid || V[e] == centroid) continue;
		if (uf.merge(U[e], V[e]).second >= Asz) {
			Astart = U[e];
			goto found_a;
		}
	}

	return vector<int>(N, 0);

found_a:
	vector<int> res(N, Clabel);
	{
		vector<bool> vis(N, false);
		q = vector<int>({Astart});
		vis[Astart] = true;
		for (int i = 0; i < Asz; i++) {
			int cur = q[i];
			res[cur] = Alabel;
			for (int nxt : adj[cur]) {
				if (uf.get_par(nxt) != uf.get_par(Astart)) continue;
				if (vis[nxt]) continue;
				vis[nxt] = true;
				q.push_back(nxt);
			}
		}
	}
	{
		vector<bool> vis(N, false);
		q = vector<int>({centroid});
		vis[centroid] = true;
		for (int i = 0; i < Bsz; i++) {
			int cur = q[i];
			res[cur] = Blabel;
			for (int nxt : adj[cur]) {
				if (uf.get_par(nxt) == uf.get_par(Astart)) continue;
				if (vis[nxt]) continue;
				vis[nxt] = true;
				q.push_back(nxt);
			}
		}
	}
	return res;
}

vector<int> find_split(int N, int Asz, int Bsz, int Csz, vector<int> U, vector<int> V) {
	vector<int> res = my_find_split(N, Asz, Bsz, Csz, U, V);
	if (res != vector<int>(N, 0)) {
		int cnt[4] = {0,0,0,0};
		for (int i = 0; i < N; i++) {
			assert(1 <= res[i] && res[i] <= 3);
			cnt[res[i]] ++;
		}
		assert(cnt[1] == Asz);
		assert(cnt[2] == Bsz);
		assert(cnt[3] == Csz);

		union_find uf(N);
		for (int e = 0; e < int(U.size()); e++) {
			if (res[U[e]] != res[V[e]]) continue;
			uf.merge(U[e], V[e]);
		}
		int numGood = 0;
		for (int z = 1; z <= 3; z++) {
			int c = 0;
			for (int i = 0; i < N; i++) {
				if (res[i] == z) {
					c += uf.get_par(i) == i;
				}
			}
			assert(c >= 1);
			if (c == 1) {
				numGood ++;
			}
		}
		assert(numGood >= 2);
	}
	return res;
}
