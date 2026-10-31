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
		string S; cin >> S;
		int64_t ans = 0;


		int msk = 0;
		for (char c : S) {
			if (c == '?') msk ^= (1 << 26);
			else msk ^= (1 << (c - 'a'));
		}

		int whole_mask = msk & ((1 << 26) - 1);
		msk = 0;

		hash_map<int, int> mp;
		mp[msk] ++;
		for (char c : S) {
			if (c == '?') msk ^= (1 << 26);
			else msk ^= (1 << (c - 'a'));
			for (int i = 0; i <= 26; i++) {
				auto it = mp.find(msk ^ whole_mask ^ (1 << 26) ^ (1 << i));
				if (it != mp.end()) ans += it->second;
			}
			mp[msk] ++;
		}
		cout << ans << '\n';
	}

	return 0;
}
