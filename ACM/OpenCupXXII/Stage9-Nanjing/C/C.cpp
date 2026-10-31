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

	int N; cin >> N;
	int K; cin >> K;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	hash_map<int, std::vector<int>> vals;
	for (int i = 0; i < N; i++) {
		vals[A[i]].push_back(i);
		if (K != 0) {
			vals[A[i] + K].push_back(i);
		}
	}

	int ans = 0;
	for (const auto& it : vals) {
		int v = it.first;
		int dp0 = 0;
		int dp1 = 0;
		int dp2 = 0;
		for (int i : it.second) {
			if (A[i] == v) {
				dp0++;
				dp2++;
			} else if (A[i] + K == v) {
				dp1++;
			} else assert(false);
			dp1 = std::max(dp1, dp0);
			dp2 = std::max(dp2, dp1);
		}
		ans = std::max(ans, dp2);
	}
	cout << ans << '\n';

	return 0;
}
