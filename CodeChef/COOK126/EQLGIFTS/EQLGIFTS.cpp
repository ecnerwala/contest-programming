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

void go() {
	using namespace std;

	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;
	//mt19937 mt(48); vector<int> A(N); for (auto& a : A) a = int(mt() % int(1e8)) + 1;
	vector<int> B(N);
	vector<int> C(N);
	hash_map<int64_t, int> mp;
	vector<int> ans(N);
	for (int s = 1; s <= 50000; s++) {
		copy(A.begin(), A.end(), B.begin());
		std::shuffle(B.begin(), B.end(), mt19937(s));
		int64_t v = 0;
		for (int i = 0; i+1 < N; i++) {
			v += B[i];
			auto [it, did] = mp.insert({v, s});
			if (!did) {
				fill(ans.begin(), ans.end(), 0);
				int num_plus = 0;

				iota(C.begin(), C.end(), 0);
				std::shuffle(C.begin(), C.end(), mt19937(it->second));
				int64_t cur = v;
				for (int idx : C) {
					cur -= A[idx];
					ans[idx] ++;
					num_plus++;
					if (cur == 0) break;
				}
				iota(C.begin(), C.end(), 0);
				std::shuffle(C.begin(), C.end(), mt19937(s));
				for (int j = 0; j <= i; j++) {
					int idx = C[j];
					if (ans[idx]) num_plus--;
					ans[idx] --;
				}
				if (!num_plus) continue;

				cout << "YES" << '\n';
				for (int x : ans) cout << "ANB"[x+1];
				cout << '\n';
				return;
			}
		}
	}

	cout << "NO" << '\n';
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		go();
	}

	return 0;
}
