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
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int64_t> A(N);
		std::vector<int> cost(N);
		std::vector<hash_set<int64_t>> good_cost(N);
		for (int cur = N-1; cur >= 0; cur--) {
			std::cin >> A[cur];
			int num_ch; std::cin >> num_ch;
			for (int z = 0; z < num_ch; z++) {
				int nxt; std::cin >> nxt; nxt--;
				cost[cur] += cost[nxt];
				for (auto v : good_cost[nxt]) {
					int64_t nv = std::gcd(v, A[cur]);
					if (nv > 1) good_cost[cur].insert(nv);
				}
			}
			if (good_cost[cur].empty()) {
				cost[cur]++;
				good_cost[cur].insert(A[cur]);
			}
			std::cout << cost[cur] << '\n' << std::flush;
		}
	}

	return 0;
}
