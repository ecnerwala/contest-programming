#include<bits/extc++.h>

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

	std::array<std::array<char, 8>, 8> grid;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cin >> grid[i][j];
		}
	}
	std::set<std::string> words;
	int D; cin >> D;
	while (D--) {
		string s; cin >> s;
		words.insert(s);
	}

	std::vector<std::vector<uint64_t>> msks(64);

	for (int small_msk = 0; small_msk < (1 << 16); small_msk++) {
		int S = __builtin_popcount(small_msk);
		if (S < 3 || S > 4) continue;
		if (!(small_msk & 0b1111)) continue; // must have top row
		if (!(small_msk & 0x1111)) continue; // must have left column
		std::vector<std::array<int, 2>> cells; cells.reserve(S);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (small_msk & (1 << (i*4+j))) {
					cells.push_back({i, j});
				}
			}
		}
		std::vector<int> par(S, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};
		int num_cc = S;
		for (int x = 0; x < S; x++) {
			for (int y = x+1; y < S; y++) {
				if (std::abs(cells[x][0] - cells[y][0]) + std::abs(cells[x][1] - cells[y][1]) == 1) {
					num_cc -= merge(x, y);
				}
			}
		}
		if (num_cc != 1) continue;
		//cerr << "Found something" << '\n';
		//for (auto [x, y] : cells) cerr << x << ' ' << y << '\n';
		int max_x = 0;
		int max_y = 0;
		for (auto [x, y] : cells) {
			max_x = std::max(max_x, x);
			max_y = std::max(max_y, y);
		}
		for (int a = 0; a+max_x < 8; a++) {
			for (int b = 0; b+max_y < 8; b++) {
				uint64_t msk = 0;
				std::string s;
				for (auto [x, y] : cells) {
					s += grid[a+x][b+y];
					msk |= uint64_t(1) << ((a+x)*8+(b+y));
				}
				if (words.count(s)) {
					msks[__builtin_ctzll(msk)].push_back(msk);
				}
			}
		}
	}

	int64_t ans = 0;
	std::vector<hash_map<uint64_t, int64_t>> q(64);
	auto insert = [&](uint64_t v, int64_t w) {
		if (v == 0) ans += w;
		else q[__builtin_ctzll(v)][v] += w;
	};
	insert(~uint64_t(0), 1);
	for (int b = 0; b < 64; b++) {
		for (auto [v, w] : q[b]) {
			for (auto msk : msks[b]) {
				if ((v & msk) == msk) {
					insert(v ^ msk, w);
				}
			}
		}
	}

	cout << ans << '\n';

	return 0;
}
