#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, Q; cin >> N >> M >> Q;
		std::vector<std::vector<int>> out_edges(N);
		std::vector<std::vector<int>> in_edges(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			out_edges[u].push_back(v);
			in_edges[v].push_back(u);
		}
		// values are
		// 0 - blue
		// 1 - first
		// 2 - second
		// 3 - red
		std::vector<int> val(N, -1);
		std::vector<std::array<int, 4>> ch_freq(N, {0, 0, 0, 0});
		std::vector<bool> is_red(N, false);
		auto get_val = [&](int v) -> int {
			if (is_red[v]) {
				return 3;
			} else if (int(out_edges[v].size()) == 0) {
				return 0;
			} else if (ch_freq[v][2] || (ch_freq[v][0] && ch_freq[v][3])) {
				return 1;
			} else if (ch_freq[v][3]) {
				return 3;
			} else if (ch_freq[v][0]) {
				return 0;
			} else {
				return 2;
			}
		};
		{
			std::vector<int> q; q.reserve(N);
			std::vector<int> out_deg(N);
			for (int v = 0; v < N; v++) {
				out_deg[v] = int(out_edges[v].size());
				if (out_deg[v] == 0) {
					q.push_back(v);
				}
			}
			for (int z = 0; z < int(q.size()); z++) {
				int v = q[z];
				val[v] = get_val(v);
				for (int u : in_edges[v]) {
					ch_freq[u][val[v]]++;
					if (!--out_deg[u]) q.push_back(u);
				}
			}
		}
		auto update = [&](this auto&& self, int v) -> void {
			int old_val = val[v];
			int new_val = val[v] = get_val(v);
			assert(new_val >= old_val);
			if (new_val == old_val) return;
			for (int u : in_edges[v]) {
				ch_freq[u][old_val]--;
				ch_freq[u][new_val]++;
				self(u);
			}
		};
		for (int q = 0; q < Q; q++) {
			int op, v; cin >> op >> v; v--;
			if (op == 1) {
				// change the node to red
				assert(!is_red[v]);
				is_red[v] = true;
				update(v);
			} else if (op == 2) {
				// query 
				cout << (val[v] <= 1 ? "YES" : "NO") << '\n';
			} else assert(false);
		}
	}

	return 0;
}

// If a node has 2nd node, or both Red and Blue, then it's 1st
// If a node has only R and 1st, then it's R
// If a node has only B and 1st, then it's B
// If a node has only 1st, then it's 2nd
//
// When a node recolors to R:
// if we convert from 2nd to R, we can convert from 1st to R, or stay 1st
// if we convert from 1st to R, we can convert from 2nd to R, or B to 1st
// if we convert from B to 1st, we can convert from B to 2nd, or 1st to R
// if we convert from B to 2nd, we can convert from B to 1st, or stay 1st
// B to R is just a composition of those 2
