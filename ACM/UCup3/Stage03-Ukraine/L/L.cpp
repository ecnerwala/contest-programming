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

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::vector<std::pair<int, int>>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back({v, e});
			adj[v].push_back({u, e});
		}
		std::vector<int> depth(N, -1);
		std::vector<int> sz(N, 0);
		std::vector<int> par(N, -1);
		std::vector<int> cyc_root(N, -1);
		depth[0] = 0;
		std::y_combinator([&](auto self, int cur, int prv_e) -> void {
			sz[cur] = 1;
			for (auto [nxt, nxt_e] : adj[cur]) {
				if (nxt_e == prv_e) continue;
				if (depth[nxt] == -1) {
					par[nxt] = cur;
					depth[nxt] = depth[cur] + 1;
					self(nxt, nxt_e);
					sz[cur] += sz[nxt];
				} else if (depth[nxt] < depth[cur]) {
					cyc_root[cur] = nxt;
				} else {
					assert(cyc_root[nxt] == cur);
				}
			}
		})(0, -1);

		int64_t ans = 0;
		{
			std::vector<int> buf; buf.reserve(N);
			for (int st = 0; st < N; st++) {
				if (cyc_root[st] != -1) {
					buf.clear();
					for (int cur = st; cur != cyc_root[st]; cur = std::exchange(par[cur], -1)) {
						buf.push_back(sz[cur]);
					}
					buf.push_back(N);
					for (int i = int(buf.size()) - 1; i > 0; i--) buf[i] -= buf[i-1];

					int L = int(buf.size());

					assert(std::accumulate(buf.begin(), buf.end(), 0) == N);

					int num_match = 0;
					{
						auto it = std::max_element(buf.begin(), buf.end());
						if (N - *it < *it) {
							*it = N - *it;
							num_match = N - *it;
						} else {
							num_match = N / 2;
						}
					}
					assert(num_match * 2 == std::accumulate(buf.begin(), buf.end(), 0));
					// now split the sequence into two parts
					std::vector<int> part_1(L, 0);
					std::vector<int> part_2(L, 0);
					int p1_sz = num_match;
					for (int i = 0; i < L; i++) {
						int v1 = std::min(p1_sz, buf[i]);
						part_1[i] = v1;
						p1_sz -= v1;
						part_2[i] = buf[i] - v1;
					}
					for (int i = 0, j = 0; i < L || j < L; ) {
						if (i < L && part_1[i] == 0) { i++; continue; }
						if (j < L && part_2[j] == 0) { j++; continue; }
						assert(i < L && part_1[i] != 0);
						assert(j < L && part_2[j] != 0);
						int v = std::min(part_1[i], part_2[j]);
						part_1[i] -= v;
						part_2[j] -= v;
						ans += int64_t(v) * int64_t(std::min(std::abs(i - j), L - std::abs(i - j)));
					}
				}
			}
		}
		for (int i = 0; i < N; i++) {
			if (par[i] != -1) {
				ans += std::min(sz[i], N - sz[i]);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
