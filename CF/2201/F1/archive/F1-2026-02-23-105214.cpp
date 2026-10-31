#include <bits/stdc++.h>

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		std::array<std::vector<int>, 2> row_freq;
		std::array<std::vector<int>, 2> freq_freq;
		std::array<uint64_t, 2> hsh;
		for (int z = 0; z < 2; z++) {
			row_freq[z].resize(N);
			freq_freq[z].resize(N);
			hsh[z] = 0;
		}
		for (int q = 0; q < Q; q++) {
			for (int z = 0; z < 2; z++) {
				int r; cin >> r; r--;
				int f = row_freq[z][r]++;
				int g = freq_freq[z][f]++;
				if (z) std::swap(f, g);
				hsh[z] += splitmix64_hash::splitmix64((uint64_t(f) << 32) | uint64_t(g));
			}
			cout << (hsh[0] == hsh[1] ? "YES" : "NO") << '\n';
		}
	}

	return 0;
}
