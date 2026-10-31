#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int> weight(N, 1);
	std::vector<int> in_deg(N);
	std::vector<bool> has_self_edge(N);

	std::vector<std::set<int>> in_edges(N);
	std::vector<std::set<int>> out_edges(N);

	auto add_edge = [&](int u, int v) -> void {
		if (u == v) {
			if (has_self_edge[u]) return;
			has_self_edge[u] = true;
			in_deg[u]++;
		}
		if (out_edges[u].count(v)) return;
		out_edges[u].insert(v);
		in_edges[v].insert(u);
		in_deg[v]++;
	};
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		add_edge(u, v);
	}

	int base_ans = 0;
	{
		std::vector<int> q; q.reserve(N);
		for (int i = 0; i < N; i++) {
			if (in_deg[i] == 0) q.push_back(i);
		}
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			cerr << "erase " << cur << '\n';
			assert(weight[cur] == 1);
			base_ans += weight[cur];
			weight[cur] = 0;
			assert(!has_self_edge[cur]);
			assert(in_edges[cur].empty());
			for (int nxt : out_edges[cur]) {
				assert(nxt != cur);
				in_edges[nxt].erase(cur);
				in_deg[nxt]--;
				assert(in_deg[nxt] >= 0);
				if (!in_deg[nxt]) q.push_back(nxt);
			}
		}
	}

	// now, everything has indeg >= 1
	std::vector<int> q; q.reserve(2*N);
	for (int i = 0; i < N; i++) {
		if (weight[i] == 0) continue;
		assert(in_deg[i] >= 1);
		if (in_deg[i] == 1 && !has_self_edge[i]) q.push_back(i);
	}
	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		cerr << "q " << cur << '\n';
		if (weight[cur] == 0) continue;
		assert(in_deg[cur] == 1);
		if (has_self_edge[cur]) {
			continue;
		}
		assert(int(in_edges[cur].size()) == 1);
		int prv = *in_edges[cur].begin();
		cerr << "merge " << cur << ' ' << prv << '\n';
		assert(prv != cur);
		assert(weight[prv] > 0);
		in_edges[cur].erase(prv);
		out_edges[prv].erase(cur);
		if (out_edges[cur].count(prv)) {
			// let's eagerly make this a self loop, to save ourselves some hassle
			out_edges[cur].erase(prv);
			in_edges[prv].erase(cur);
			if (!has_self_edge[prv]) {
				has_self_edge[prv] = true;
			} else {
				in_deg[prv]--;
				// has a self loop, no need to insert it into the queue
				assert(in_deg[prv] >= 1);
				assert(has_self_edge[prv]);
			}
		}
		bool did_swap;
		if (int(in_edges[cur].size()) + int(out_edges[cur].size()) > int(in_edges[prv].size()) + int(out_edges[prv].size())) {
			did_swap = true;
			std::swap(cur, prv);
		}
		cerr << "  merge into " << prv << '\n';
		weight[prv] += weight[cur];
		weight[cur] = 0;
		// merge cur into prv
		for (int nxt : out_edges[cur]) {
			assert(nxt != cur && nxt != prv);
			in_edges[nxt].erase(cur);
			if (out_edges[prv].count(nxt)) {
				in_deg[nxt]--;
				assert(in_deg[nxt] >= 1);
				if (in_deg[nxt] == 1) {
					q.push_back(nxt);
				}
			} else {
				out_edges[prv].insert(nxt);
				in_edges[nxt].insert(prv);
			}
		}
		out_edges[cur] = {};
		for (int nxt : in_edges[cur]) {
			assert(nxt != cur && nxt != prv);
			out_edges[nxt].erase(cur);
			if (in_edges[prv].count(nxt)) {
				// do nothing
			} else {
				in_deg[prv]++;
				in_edges[prv].insert(nxt);
				out_edges[nxt].insert(prv);
			}
		}
		in_edges[cur] = {};
		if (has_self_edge[cur]) {
			if (!has_self_edge[prv]) {
				has_self_edge[prv] = true;
				in_deg[prv]++;
			}
		}
		has_self_edge[cur] = false;
		in_deg[cur] = 0;
		if (did_swap && in_deg[prv] == 1 && !has_self_edge[prv]) q.push_back(prv);
	}

	cout << base_ans + *std::max_element(weight.begin(), weight.end()) << '\n';

	return 0;
}
