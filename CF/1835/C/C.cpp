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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

	int T; cin >> T;
	while (T--) {
		int K; cin >> K;
		int N = 1 << (K+1);
		std::vector<int64_t> G(N);
		for (auto& g : G) cin >> g;

		std::vector<int64_t> pref(N+1);
		for (int i = 0; i < N; i++) {
			pref[i+1] = pref[i] ^ G[i];
		}

		auto solve = [&]() -> std::optional<std::array<int, 4>> {
			std::vector<std::pair<int64_t, int>> pref_sorted(N+1);
			for (int i = 0; i <= N; i++) {
				pref_sorted[i] = {pref[i], i};
			}
			std::sort(pref_sorted.begin(), pref_sorted.end());

			for (int i = 0; i <= N; i++) {
				for (int j = i+1; j <= N && j <= i+2; j++) {
					if (pref_sorted[j].first == pref_sorted[i].first && pref_sorted[j].second - pref_sorted[i].second >= 2) {
						int a = pref_sorted[i].second;
						int b = pref_sorted[j].second;
						return {{a, a+1, a+1, b}};
					}
				}
			}

			int loc_0 = -1;
			for (int i = 0; i < N; i++) {
				if (G[i] == 0) {
					if (loc_0 == -1) {
						loc_0 = i;
					} else {
						return {{loc_0, loc_0 + 1, i, i+1}};
					}
				}
			}

			// now, there's at most one equal pair and it's guaranteed to be useless
			for (int i = 0; i+1 <= N; i++) {
				if (pref_sorted[i].first == pref_sorted[i+1].first) {
					assert(pref_sorted[i+1].second - pref_sorted[i].second == 1);
					assert(pref_sorted[i].second == loc_0);
					pref_sorted.erase(pref_sorted.begin() + i + 1);
					break;
				}
			}

			hash_map<int64_t, int> vals(pref_sorted.begin(), pref_sorted.end());
			while (true) {
				int a = int(rng() % int(pref_sorted.size()));
				int b = int(rng() % int(pref_sorted.size()));
				if (a == b) continue;
				for (int c = 0; c < int(pref_sorted.size()); c++) {
					if (a == c || b == c) continue;
					auto it = vals.find(pref_sorted[a].first ^ pref_sorted[b].first ^ pref_sorted[c].first);
					if (it != vals.end()) {
						return {{it->second, pref_sorted[a].second, pref_sorted[b].second, pref_sorted[c].second}};
					}
				}
			}
		};

		auto ans = solve();
		if (ans) {
			auto a = *ans;
			std::sort(a.begin(), a.end());
			assert(a[0] < a[1] && a[2] < a[3]);
			cout << a[0]+1 << ' ' << a[1] << ' ' << a[2]+1 << ' ' << a[3] << '\n';
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
