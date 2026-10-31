#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int M; cin >> M;
		const int64_t INF = 1e18;
		int64_t num_take = 0;
		int64_t num_optional = 0;
		hash_map<int64_t, std::pair<int64_t, int64_t>> take;
		for (int m = 0; m < M; m++) {
			int n; int64_t l, r; cin >> n >> l >> r;
			std::vector<int64_t> A(n);
			for (auto& a : A) cin >> a;
			std::vector<int64_t> C(n);
			int64_t tot_c = 0;
			for (auto& c : C) { cin >> c; tot_c += c; }

			num_optional = std::min(num_optional + (r - l), INF);
			num_take = std::min(num_take + l, INF);

			for (int i = 0; i < n; i++) {
				auto& v = take[A[i]];
				int64_t num_others = std::min(tot_c - C[i], r);
				if (num_others <= l) {
					v.first = std::min(v.first + (l - num_others), INF);
					v.second = std::min(v.second + (r - l), INF);
				} else {
					v.second = std::min(v.second + (r - num_others), INF);
				}
			}
		}
		//cerr << "num_take: " << num_take << '\n';
		//cerr << "num_optional: " << num_optional << '\n';
		int64_t best = INF;
		int64_t num_cnds = 0;
		for (auto [l, v] : take) {
			//cerr << l << ' ' << v.first << ' ' << v.second << '\n';
			if (l < num_take) continue;
			if (l > num_take + num_optional) continue;
			num_cnds ++;

			int64_t cnd = v.first + std::max<int64_t>(l - num_take - (num_optional - v.second), 0);
			//cerr << l << ' ' << v.first << ' ' << v.second << ' ' << cnd << '\n';

			best = std::min(best, cnd);
		}
		if (num_cnds < num_optional + 1) {
			best = 0;
		}
		cout << best << '\n';
	}

	return 0;
}
