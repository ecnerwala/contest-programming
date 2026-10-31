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
	hash_map<int, int> rows;
	hash_map<int, bool> cols;
	for (int i = 0; i < N; i++) {
		int x, y; cin >> x >> y;
		rows[x] ^= (y % 3) + 1; // Klein 4 group
		cols[y] ^= 1;
	}

	array<int, 3> num_cols({0,0,0});
	for (const auto& it : cols) {
		if (it.second) num_cols[it.first % 3]++;
	}
	array<int, 3> num_rows({0,0,0});
	for (const auto& it : rows) {
		if (it.second) num_rows[it.second - 1] ++;
	}

	// 0-rows and 0-cols, and those be fulfilled by 1 cell
	// 0-row and 1-col and 2-col can together be fulfilled by 2 cells
	// And cyclic
	// Also, you can create/delete pairs of k-cols

	// we can add 2 to any entry of num_cols
	int ans = N;
	for (int z = 0; z < 3; z++) {
		int cnd = 0;
		int x = (z+1)%3;
		int y = (z+2)%3;
		int ex = (max(0, num_rows[x] - num_cols[x]) + 1) / 2 * 2;
		int ey = (max(0, num_rows[y] - num_cols[y]) + 1) / 2 * 2;
		int ez = max(0, num_rows[z] - num_cols[z] - min(ex + num_cols[x] - num_rows[x], ey + num_cols[y] - num_rows[y]));
		assert((ez % 2) == 0);
		cnd = num_cols[z] + ez + num_cols[x] + ex + num_cols[y] + ey;
		ans = min(ans, cnd);
	}
	cout << ans << '\n';

	return 0;
}

// (x, y) -> (-1, y), one or both of (x, -1) and (x, -2) and the same group of (-1, -1) and (-1, -2)
// (-1, -1) and (-1, -2) don't really matter, because they're always fulfilled as long as the other constraints are
