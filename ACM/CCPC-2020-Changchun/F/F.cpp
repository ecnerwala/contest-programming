#include <bits/extc++.h>

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
	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;

	using cnt_t = array<array<int, 2>, 20>;
	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	int64_t ans = 0;

	std::y_combinator([&](auto self, int cur, int prv) -> hash_map<int, cnt_t> {
		hash_map<int, cnt_t> res;
		{
			auto& c = res[A[cur]];
			for (int i = 0; i < 20; i++) {
				c[i][((cur+1) >> i) & 1]++;
			}
		}
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			auto v = self(nxt, cur);
			if (v.size() > res.size()) res.swap(v);
			for (auto it : v) {
				if (res.find(A[cur] ^ it.first) == res.end()) continue;
				const auto& x = it.second;
				const auto& y = res[A[cur] ^ it.first];

				for (int i = 0; i < 20; i++) {
					ans += (int64_t(x[i][0]) * y[i][1] + int64_t(x[i][1]) * y[i][0]) << i;
				}
			}
			for (auto it : v) {
				const auto& x = it.second;
				auto& y = res[it.first];
				for (int i = 0; i < 20; i++) {
					y[i][0] += x[i][0];
					y[i][1] += x[i][1];
				}
			}
		}
		return res;
	})(0, -1);

	cout << ans << '\n';

	return 0;
}
