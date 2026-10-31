#include<bits/stdc++.h>

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

	int N, M, K; cin >> N >> M >> K;
	vector<vector<pair<int, int>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].emplace_back(v, e);
		adj[v].emplace_back(u, e);
	}
	vector<int> sz(N);
	std::y_combinator([&](auto self, int cur, int prv) -> int {
		for (auto it = adj[cur].begin(); it != adj[cur].end(); ++it) {
			if (it->first == prv) {
				adj[cur].erase(it);
				break;
			}
		}

		int res = 1;
		for (auto [nxt, e] : adj[cur]) {
			res += self(nxt, cur);
		}
		sort(adj[cur].begin(), adj[cur].end(), [&](const auto& a, const auto& b) { return sz[a.first] > sz[b.first]; });
		return sz[cur] = res;
	})(0, -1);

	int next_idx = 0;

	vector<int> preorder_idx(N);
	vector<int> preorder_par(N);
	vector<int> preorder_edges(N);
	vector<int> depth(N);
	vector<int> heavy_root(N);

	preorder_idx[0] = next_idx;
	preorder_par[next_idx] = -1;
	preorder_edges[next_idx] = -1;
	depth[next_idx] = 0;
	heavy_root[next_idx] = 0;
	next_idx++;
	std::y_combinator([&](auto self, int cur, int cur_idx) -> void {
		bool is_heavy = true;
		for (auto [nxt, e] : adj[cur]) {
			preorder_idx[nxt] = next_idx;
			preorder_par[next_idx] = cur_idx;
			preorder_edges[next_idx] = e;
			depth[next_idx] = depth[cur_idx] + 1;
			heavy_root[next_idx] = is_heavy ? heavy_root[cur_idx] : next_idx;
			self(nxt, next_idx++);
			is_heavy = false;
		}
	})(0, 0);

	vector<int> pref(N+1);

	for (int m = 0; m < M; m++) {
		int L; cin >> L;
		vector<int> locs(L);
		for (auto& a : locs) {
			int v; cin >> v; v--;
			a = preorder_idx[v];
		}
		sort(locs.begin(), locs.end());
		for (int i = 0; i < int(locs.size()); i++) {
			int a = locs[i];
			int b = locs[i+1 == int(locs.size()) ? 0 : i+1];
			while (true) {
				if (a > b) swap(a, b);
				assert(a <= b);
				if (heavy_root[b] <= a) {
					pref[a+1]++;
					pref[b+1]--;
					break;
				} else {
					pref[heavy_root[b]]++;
					pref[b+1]--;
					b = preorder_par[heavy_root[b]];
				}
			}
		}
	}

	vector<int> ans; ans.reserve(N-1);
	for (int i = 0; i < N; i++) {
		pref[i+1] += pref[i];
	}
	for (int i = 0; i <= N; i++) {
		assert(pref[i] % 2 == 0);
		pref[i] /= 2;
	}
	assert(pref[0] == 0);
	assert(pref[N] == 0);

	for (int i = 0; i < N; i++) {
		if (pref[i] >= K) {
			assert(i);
			ans.push_back(preorder_edges[i]);
		}
	}
	sort(ans.begin(), ans.end());

	cout << ans.size() << '\n';
	for (int i = 0; i < int(ans.size()); i++) {
		cout << ans[i]+1 << " \n"[i+1==int(ans.size())];
	}

	return 0;
}
