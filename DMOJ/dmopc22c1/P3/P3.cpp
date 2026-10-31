#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, Q; cin >> N >> M >> Q;
	std::vector<int> deg(N);
	std::vector<int> edge_xor(N);
	int64_t tot_1_deg = 0;
	int64_t tot_more_deg = 0;
	int num_1_1 = 0;
	std::set<std::array<int, 2>> edges;
	for (int e = 0; e < M+Q; e++) {
		bool is_stars = tot_1_deg - 2 * num_1_1 == tot_more_deg;
		//cerr << (!is_stars ? "YES" : "NO") << '\n';
		if (e >= M) {
			cout << (!is_stars ? "YES" : "NO") << '\n';
		}
		int u, v; cin >> u >> v; u--, v--;
		assert(u != v);
		if (u > v) std::swap(u, v);
		assert(u < v);
		if (edges.count({u, v})) {
			assert(e >= M);
			// erase u, v
			edges.erase({u, v});
			(deg[u] == 1 ? tot_1_deg : tot_more_deg) -= deg[u];
			(deg[v] == 1 ? tot_1_deg : tot_more_deg) -= deg[v];
			deg[u]--;
			deg[v]--;
			(deg[u] == 1 ? tot_1_deg : tot_more_deg) += deg[u];
			(deg[v] == 1 ? tot_1_deg : tot_more_deg) += deg[v];
			edge_xor[u] ^= v;
			edge_xor[v] ^= u;
			if (deg[u] == 0 && deg[v] == 0) {
				num_1_1--;
			}
			if (deg[u] == 1 && deg[edge_xor[u]] == 1) {
				num_1_1++;
			}
			if (deg[v] == 1 && deg[edge_xor[v]] == 1) {
				num_1_1++;
			}
		} else {
			// insert u and v
			if (deg[u] == 0 && deg[v] == 0) {
				num_1_1++;
			}
			if (deg[u] == 1 && deg[edge_xor[u]] == 1) {
				num_1_1--;
			}
			if (deg[v] == 1 && deg[edge_xor[v]] == 1) {
				num_1_1--;
			}
			edges.insert({u, v});
			(deg[u] == 1 ? tot_1_deg : tot_more_deg) -= deg[u];
			(deg[v] == 1 ? tot_1_deg : tot_more_deg) -= deg[v];
			deg[u]++;
			deg[v]++;
			(deg[u] == 1 ? tot_1_deg : tot_more_deg) += deg[u];
			(deg[v] == 1 ? tot_1_deg : tot_more_deg) += deg[v];
			edge_xor[u] ^= v;
			edge_xor[v] ^= u;
		}
		assert(tot_1_deg + tot_more_deg == int(edges.size()) * 2);
		assert(tot_more_deg >= tot_1_deg - 2 * num_1_1);
	}
	bool is_stars = tot_1_deg - 2 * num_1_1 == tot_more_deg;
	//cerr << (!is_stars ? "YES" : "NO") << '\n';
	cout << (!is_stars ? "YES" : "NO") << '\n';

	return 0;
}
