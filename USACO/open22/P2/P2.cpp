#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::array<int, 2>> edges(M);
	for (auto& e : edges) for (auto& v : e) { cin >> v; v--; }

	std::vector<std::set<int>> outEdges(N);
	std::vector<std::set<int>> inEdges(N);
	for (auto [u, v] : edges) {
		outEdges[u].insert(v);
		inEdges[v].insert(u);
	}

	std::vector<bool> is_dead_end(N, false);

	std::vector<int> q; q.reserve(N);
	for (int i = 0; i < N; i++) {
		if (outEdges[i].size() == 0) q.push_back(i);
	}
	for (int z = 0; z < int(q.size()); z++) {
		int i = q[z];
		is_dead_end[i] = true;
		for (int j : inEdges[i]) {
			outEdges[j].erase(i);
			if (outEdges[j].size() == 0) q.push_back(j);
		}
		inEdges[i].clear();
	}

	q.clear();
	for (int i = 0; i < N; i++) {
		if (outEdges[i].size() == 1) q.push_back(i);
	}

	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};

	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		cur = get_par(cur);
		assert(outEdges[cur].size() == 1);
		assert(par[cur] < 0);
		int nxt = *outEdges[cur].begin();
		assert(par[nxt] < 0);

		if (cur == nxt) continue;

		outEdges[cur].clear();
		inEdges[nxt].erase(cur);

		bool add_self_loop = false;
		if (inEdges[cur].count(nxt)) {
			inEdges[cur].erase(nxt);
			outEdges[nxt].erase(cur);
			add_self_loop = true;
		}
		if(inEdges[nxt].count(nxt)) {
			inEdges[nxt].erase(nxt);
			outEdges[nxt].erase(nxt);
			add_self_loop = true;
		}

		if (inEdges[nxt].size() + outEdges[nxt].size() < inEdges[cur].size()) {
			std::swap(cur, nxt);
		}
		// redirect outedges
		if (!outEdges[cur].empty()) {
			assert(outEdges[nxt].empty());
			for (int n : outEdges[cur]) {
				// only one has outedges so we don't need to dedup
				inEdges[n].erase(cur);
				inEdges[n].insert(nxt);
			}
			outEdges[nxt] = std::move(outEdges[cur]);
		}
		for (int p : inEdges[cur]) {
			assert(p != cur && p != nxt);
			outEdges[p].erase(cur);
			if (inEdges[nxt].count(p)) {
				assert(outEdges[p].count(nxt));
				if (outEdges[p].size() == 1) {
					q.push_back(p);
				}
			} else {
				outEdges[p].insert(nxt);
				inEdges[nxt].insert(p);
			}
		}
		inEdges[cur].clear();

		// merge cur into nxt
		if (add_self_loop) {
			inEdges[nxt].insert(nxt);
			outEdges[nxt].insert(nxt);
		}

		par[cur] = nxt;
	}

	int Q; cin >> Q;
	while (Q--) {
		int u, v; cin >> u >> v; u--, v--;
		if (is_dead_end[u] || is_dead_end[v] || get_par(u) == get_par(v)) {
			cout << 'B';
		} else {
			cout << 'H';
		}
	}
	cout << '\n';

	return 0;
}
