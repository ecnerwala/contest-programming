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

	const int64_t OFFSET = int64_t(1) << 30;
	const int64_t B = int64_t(1) << 31;

	hash_map<int64_t, int64_t> adj;
	pair<int64_t, int64_t> first_pt;
	for (int i = 0; i < N; i++) {
		int64_t x, y; cin >> x >> y;
		assert(x % 2 == 0 && y % 2 == 0);
		int64_t p = (x + OFFSET) * B + (y + OFFSET);
		vector<int64_t> pts; pts.reserve(8);
		if ((x ^ y) & 2) {
			// octagon
			pts = {
				p - B,
				p - 1,
				p + B - 1,
				p + 2*B,
				p + 2*B + 1,
				p + B + 2,
				p + 2,
				p - B + 1
			};
		} else {
			// square
			pts = {
				p,
				p + B,
				p + B + 1,
				p + 1,
			};
		}
		for (int z = 0; z < int(pts.size()); z++) {
			int64_t a = pts[z], b = pts[(z+1)%int(pts.size())];
			adj[a] ^= b;
			adj[b] ^= a;
		}
		if (i == 0 || make_pair(pts[0], pts.back()) < first_pt) {
			first_pt = make_pair(pts[0], pts.back());
		}
	}

	vector<int64_t> resp;
	int64_t cur, nxt; tie(cur, nxt) = first_pt;
	while (resp.empty() || cur != first_pt.first) {
		resp.push_back(cur);
		std::swap(cur, nxt);
		nxt ^= adj[cur];
	}

	cout << resp.size() << '\n';
	for (auto p : resp) {
		cout << (p / B - OFFSET) << ' ' << (p % B - OFFSET) << '\n';
	}

	return 0;
}
