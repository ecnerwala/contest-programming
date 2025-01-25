#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	int ROOT = 17289082517 % N;
	std::y_combinator([&](auto self, int cur, int prv) -> int {
		int sz = 1;
		std::pair<int, int> best_ch(0, -1);
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			int ch_sz = self(nxt, cur);
			sz += ch_sz;
			if (ch_sz > best_ch.first) {
				best_ch = {ch_sz, nxt};
			}
		}
		if (best_ch.first) {
			auto it = std::find(adj[cur].begin(), adj[cur].end(), best_ch.second);
			std::swap(*adj[cur].begin(), *it);
		}
		return sz;
	})(ROOT, -1);
	std::vector<int> preorder_idx(N, -1);
	std::vector<std::pair<int, int>> heavy_root(N);
	std::vector<int> A(N);
	{
		int node_idx = 0;
		std::y_combinator([&](auto self, int cur, int prv, std::pair<int, int> cur_heavy_root) -> void {
			int cur_idx = node_idx++;
			preorder_idx[cur] = cur_idx;
			heavy_root[cur] = cur_heavy_root;
			A[cur_idx] = cur+1;
			bool is_first = true;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur, is_first ? cur_heavy_root : std::pair<int, int>{node_idx, cur_idx});
				is_first = false;
			}
		})(ROOT, -1, std::pair<int, int>{node_idx, -1});
		assert(node_idx == N);
	}

	for (int q = 0; q < Q; q++) {
		char op;
		int a, v, x;
		cin >> op >> a >> v; a--, v--;
		a = preorder_idx[a], v = preorder_idx[v];
		if (op == '+') cin >> x;
		else if (op == '?') { x = 0; }
		else assert(false);

		auto process_range = [&](int l, int r) -> void {
			if (op == '+') {
				for (int i = l; i < r; i++) {
					A[i] += x;
				}
			} else if (op == '?') {
				for (int i = l; i < r; i++) {
					x ^= A[i];
				}
			} else assert(false);
		};
		while (true) {
			if (a > v) std::swap(a, v);
			if (a >= heavy_root[v].first) {
				process_range(a, v+1);
				break;
			}
			process_range(heavy_root[v].first, v+1);
			v = heavy_root[v].second;
		}
		if (op == '?') cout << x << '\n';
	}

	return 0;
}
