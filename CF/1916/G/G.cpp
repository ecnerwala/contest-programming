#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

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
		int N; cin >> N;
		std::vector<std::vector<std::pair<int, int64_t>>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; int64_t w; cin >> u >> v >> w; u--, v--;
			adj[u].push_back({v, w});
			adj[v].push_back({u, w});
		}

		// Answer is always at least max w, so we only care about gcds >= that
		// You need long paths to beat any particular w
		// In particular, you need at least one collision...

		int64_t ans = 0;
		std::vector<bool> alive(N, true);
		std::vector<int> sz(N);
		auto compute_sz = std::y_combinator([&](auto self, int cur, int prv) -> int {
			if (!alive[cur]) return sz[cur] = 0;
			int res = 1;
			for (auto [nxt, _] : adj[cur]) {
				if (nxt == prv) continue;
				res += self(nxt, cur);
			}
			return sz[cur] = res;
		});
		compute_sz(0, -1);
		std::y_combinator([&](auto self_centroid, int rt) -> void {
			cerr << "dfs " << rt << '\n';
			assert(alive[rt]);
			int tot_sz = sz[rt];
			while (true) {
				int prv = -1;
				int nxt = -1;
				for (auto [ch, _] : adj[rt]) {
					if (ch == prv) continue;
					if (sz[ch] > 2 * tot_sz) {
						assert(alive[ch]);
						nxt = ch;
						break;
					}
				}
				if (nxt == -1) break;
				prv = rt;
				rt = nxt;
			}
			cerr << "set rt to " << rt << '\n';
			compute_sz(rt, -1);

			// Now, solve everything here
			hash_map<int64_t, std::array<std::pair<int, int>, 2>> best_depth;
			std::vector<hash_map<int64_t, int>> dp; dp.reserve(adj[rt].size());
			for (auto [ch, ch_w] : adj[rt]) {
				if (!alive[ch]) continue;
				hash_map<int64_t, int> ch_best_depth;
				std::y_combinator([&](auto self, int cur, int prv, int d, int64_t cur_w) -> void {
					setmax(ch_best_depth[cur_w], d);
					setmax(ans, cur_w * d);
					for (auto [nxt, nxt_w] : adj[cur]) {
						if (nxt == prv) continue;
						self(nxt, cur, d+1, std::gcd(nxt_w, cur_w));
					}
				})(ch, rt, 1, ch_w);
				int ch_idx = int(dp.size());
				for (auto [w, d] : ch_best_depth) {
					std::pair<int, int> v(d, ch_idx + 1);
					auto& [a, b] = best_depth[w];
					if (v.first > b.first) {
						b = v;
						if (b.first > a.first) std::swap(a, b);
					}
				}
				dp.emplace_back();
				dp.back().swap(ch_best_depth);
			}
			for (int ch_idx = 0; ch_idx < int(dp.size()); ch_idx++) {
				const auto& ch_best_depth = dp[ch_idx];
				for (auto [w, d] : ch_best_depth) {
					if (w * d * 2 <= ans) continue;
					for (int k = 1; k <= d; k++) {
						auto it = best_depth.find(k * w);
						if (it != best_depth.end()) {
							int z = 0;
							if (it->second[z].second == ch_idx + 1) z++;
							setmax(ans, (d + it->second[z].first) * w);
						}
					}
				}
			}

			alive[rt] = false;
			for (auto [ch, _] : adj[rt]) {
				if (alive[ch]) self_centroid(ch);
			}
			alive[rt] = true;
		})(0);

		cout << ans << '\n';
	}

	return 0;
}
