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

	int N, M; cin >> N >> M;
	std::vector<std::vector<std::pair<int, int>>> adj(N);
	std::vector<std::array<int, 2>> endpoints(M);
	std::vector<int64_t> W(M);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v >> W[e]; u--, v--;
		endpoints[e] = {u, v};
		adj[u].push_back({e, v});
		adj[v].push_back({e, u});
	}


	{
		std::mt19937_64 mt(48);
		std::vector<uint64_t> edge_vals(M);
		for (int e = 0; e < M; e++) {
			edge_vals[e] = mt();
		}

		std::vector<bool> vis(N);
		std::vector<std::pair<int, uint64_t>> dp(N);
		std::vector<int> depth(N, -1);
		for (int i = 0; i < N; i++) {
			if (vis[i]) continue;
			depth[i] = 0;
			std::y_combinator([&](auto self, int cur, int prv_e) -> void {
				assert(!vis[cur]);
				vis[cur] = true;
				for (auto [e, nxt] : adj[cur]) {
					if (e == prv_e) continue;
					if (vis[nxt]) {
						if (depth[nxt] < depth[cur]) {
							dp[cur].first++;
							dp[cur].second ^= edge_vals[e];
							dp[nxt].first--;
							dp[nxt].second ^= edge_vals[e];
						}
					} else {
						depth[nxt] = depth[cur] + 1;
						self(nxt, e);
						if (dp[nxt].first == 0) {
							assert(dp[nxt].second == 0);
							W[e] = 0;
						}
						edge_vals[e] = dp[nxt].second;

						dp[cur].first += dp[nxt].first;
						dp[cur].second ^= dp[nxt].second;
					}
				}
			})(i, -1);
		}

		std::vector<std::pair<uint64_t, int>> edges_by_val(M);
		for (int e = 0; e < M; e++) {
			edges_by_val[e] = {edge_vals[e], e};
		}
		std::sort(edges_by_val.begin(), edges_by_val.end());

		for (int i = 0, j = 0; i < M; i = j) {
			j = i+1;
			for (; j < M && edges_by_val[j].first == edges_by_val[i].first; j++) {
				W[edges_by_val[i].second] += std::exchange(W[edges_by_val[j].second], 0);
			}
		}
	}

	int64_t ans = 0;
	for (int e = 0; e < M; e++) {
		ans = std::gcd(ans, W[e]);
	}
	if (ans == 0) {
		std::cout << ans << '\n';
		exit(0);
	}

	std::vector<int> par(2*N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};
	for (int e = 0; e < M; e++) {
		assert(W[e] % ans == 0);
		W[e] /= ans;
		auto [u, v] = endpoints[e];
		if (W[e] & 1) {
			merge(2*u+1, 2*v);
			merge(2*u, 2*v+1);
		} else {
			merge(2*u, 2*v);
			merge(2*u+1, 2*v+1);
		}
	}
	bool has_cycle = false;
	for (int i = 0; i < N; i++) {
		if (get_par(2*i) == get_par(2*i+1)) {
			has_cycle = true;
		}
	}

	if (!has_cycle) ans *= 2;
	cout << ans << '\n';

	return 0;
}
