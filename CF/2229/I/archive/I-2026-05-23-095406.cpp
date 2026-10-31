#include <bits/stdc++.h>
#include <cassert>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	const int64_t INF = 1e18;

	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;
		std::vector<int> W(N); for (auto& w : W) std::cin >> w;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; std::cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<std::vector<int64_t>> dp_down(N+1);
		auto conv_vecs = [&](const std::vector<int64_t>& a, const std::vector<int64_t>& b) -> std::vector<int64_t> {
			std::vector<int64_t> ndp(int(a.size()) + int(b.size()) - 1, -INF);
			for (int i = 0; i < int(a.size()); i++) {
				for (int j = 0; j < int(b.size()); j++) {
					setmax(ndp[i+j], a[i] + b[j]);
				}
			}
			return ndp;
		};
		[&](this auto&& self, int cur) -> void {
			dp_down[cur] = std::vector<int64_t>({0});
			for (int nxt : adj[cur]) {
				adj[nxt].erase(std::find(adj[nxt].begin(), adj[nxt].end(), cur));
				self(nxt);

				dp_down[cur] = conv_vecs(dp_down[cur], dp_down[nxt]);
			}
			dp_down[cur].push_back(-INF);
			for (int i = int(dp_down[cur].size()) - 1; i > 0; i--) {
				setmax(dp_down[cur][i], dp_down[cur][i-1] + int64_t(W[cur]) * int64_t(i));
			}
		}(0);

		auto conv_transpose = [&](const std::vector<int64_t>& a, const std::vector<int64_t>& b) -> std::vector<int64_t> {
			assert(int(a.size()) >= int(b.size()));
			std::vector<int64_t> ndp(int(a.size()) - int(b.size()) + 1, -INF);
			for (int i = 0; i < int(ndp.size()); i++) {
				for (int j = 0; j < int(b.size()); j++) {
					setmax(ndp[i], a[i+j] + b[j]);
				}
			}
			return ndp;
		};
		std::vector<int64_t> ans(N);
		std::vector<std::vector<int64_t>> dp_up(N+1);
		dp_up[0] = std::vector<int64_t>(N+1, -INF);
		dp_up[0][K] = 0;
		[&](this auto&& self, int cur) -> void {
			std::cerr << cur << ' ' << dp_down[cur].size() << ' ' << dp_up[cur].size() << '\n';
			assert(int(dp_up[cur].size()) == int(dp_down[cur].size()));
			std::vector<std::vector<int64_t>> suff_convs(adj[cur].size()+1);
			suff_convs.back() = std::vector<int64_t>({0});
			for (int z = int(adj[cur].size()) - 1; z >= 0; z--) {
				int nxt = adj[cur][z];
				suff_convs[z] = conv_vecs(suff_convs[z+1], dp_down[nxt]);
			}
			for (int z = 0; z < int(adj[cur].size()); z++) {
				int nxt = adj[cur][z];
				dp_up[cur] = conv_transpose(dp_up[cur], dp_down[nxt]);
				dp_up[nxt] = conv_transpose(dp_up[cur], suff_convs[z+1]);
				// optionally take out self from dp_up[nxt]
				for (int i = 0; i+1 < int(dp_up[nxt].size()); i++) {
					setmax(dp_up[nxt][i], dp_up[nxt][i+1] + int64_t(W[cur]) * int64_t(K - i));
				}
				dp_up[nxt].pop_back();
			}
			assert(int(dp_up[cur].size()) == 2);
			ans[cur] = dp_up[cur][1] + int64_t(W[cur]) * int64_t(K);

			for (int nxt : adj[cur]) {
				self(nxt);
			}
		}(0);
		for (int i = 0; i < N; i++) std::cout << ans[i] << " \n"[i+1==N];
	}

	return 0;
}
