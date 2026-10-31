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

	int T; cin >> T;
	while (T--) {
		int64_t H; cin >> H;
		int N; cin >> N;
		std::vector<std::pair<int64_t, int64_t>> boards(N);
		for (auto& b : boards) { cin >> b.first >> b.second; }
		int M; cin >> M;
		std::vector<std::pair<int64_t, int64_t>> coins(M);
		for (auto& b : coins) { cin >> b.first >> b.second; }

		std::sort(boards.begin(), boards.end());
		std::sort(coins.begin(), coins.end());
		auto bit = boards.begin();

		auto mod = [&](int64_t v) -> int64_t {
			v %= 2*H;
			if (v < 0) v += 2*H;
			return v;
		};

		hash_map<int64_t, int> mp;
		mp[0] = 0;

		int ans = 0;
		for (auto [cx, cy] : coins) {
			for (; bit != boards.end() && bit->first <= cx; ++bit) {
				auto [bx, by] = *bit;
				int64_t a = mod(by - bx);
				int64_t b = mod(-by - bx);
				auto at = mp.find(a);
				auto bt = mp.find(b);
				if (at == mp.end() && bt == mp.end()) {
				} else if (at == mp.end()) {
					mp.insert({a, bt->second});
				} else if (bt == mp.end()) {
					mp.insert({b, at->second});
				} else if (at->second > bt->second) {
					bt->second = at->second;
				} else {
					at->second = bt->second;
				}
			}

			for (int64_t a : {cy-cx, -cy-cx}) {
				a = mod(a);
				auto it = mp.find(a);
				if (it != mp.end()) {
					it->second++;
					ans = std::max(ans, it->second);
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
