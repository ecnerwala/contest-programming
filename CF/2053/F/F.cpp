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
		int N, M, K; cin >> N >> M >> K;
		std::vector<hash_map<int, int>> freq(N);
		std::vector<int> freq_blank(N);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				int a; cin >> a;
				if (a == -1) freq_blank[i]++;
				else freq[i][a-1]++;
			}
		}
		std::vector<hash_map<int, int>> weight(N);
		int64_t ans = 0;
		for (int i = 0; i+1 < N; i++) {
			for (auto [v, c] : freq[i]) {
				auto it = freq[i+1].find(v);
				if (it != freq[i+1].end()) ans += int64_t(c) * int64_t(it->second);
				weight[i+1][v] += c;
			}
			for (auto [v, c] : freq[i+1]) {
				weight[i][v] += c;
			}
		}
		std::vector<int64_t> val_pref(K, 0);
		std::vector<int64_t> dp_val(K, 0);
		int64_t dp_blank = 0;
		std::set<std::pair<int64_t, int>> dp_val_set;
		for (int v = 0; v < K; v++) {
			dp_val_set.insert({dp_val[v], v});
		}
		for (int i = 0; i < N; i++) {
			// when you start here, you subtract off diff_pref[i]
			for (auto [v, c] : weight[i]) {
				dp_val_set.erase({dp_val[v], v});
				dp_val[v] = std::max(dp_val[v], dp_blank) + c;
				dp_val_set.insert({dp_val[v], v});
			}
			if (i+1 < N) {
				ans += int64_t(freq_blank[i]) * int64_t(freq_blank[i+1]);
				// transition from any val to blank, at a cost of this transition
				dp_blank = std::max(dp_blank, dp_val_set.rbegin()->first - int64_t(freq_blank[i]) * int64_t(freq_blank[i+1]));
			}
		}
		ans += std::max(dp_blank, dp_val_set.rbegin()->first);
		cout << ans << '\n';
	}

	return 0;
}

// all -1's in a row become a single value
