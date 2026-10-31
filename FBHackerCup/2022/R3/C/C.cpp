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

	std::mt19937_64 mt(498);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<string> A(N);
		for (auto& s : A) cin >> s;
		int Q; cin >> Q;
		std::vector<string> B(Q);
		for (auto& s : B) cin >> s;

		int L = int(A[0].size());
		for (const auto& s : A) assert(int(s.size()) == L);
		for (const auto& s : B) assert(int(s.size()) == L);

		auto char_to_int = [&](char c) -> int {
			if (c == 'a') return 0;
			else if (c == 'm') return 1;
			else if (c == 'e') return 2;
			else if (c == 't') return 3;
			else assert(false);
		};
		for (auto& s : A) for (auto& c : s) c = char(char_to_int(c));
		for (auto& s : B) for (auto& c : s) c = char(char_to_int(c));

		cerr << "Case #" << case_num << '\n';
		cerr << "N = " << N << ' ' << "Q = " << Q << ' ' << "L = " << L << '\n';

		using hsh = uint64_t;
		std::vector<std::array<hsh, 4>> hash_ids(L);
		for (int i = 0; i < L; i++) {
			for (int z = 0; z < 4; z++) {
				hash_ids[i][z] = mt();
			}
		}

		std::vector<hsh> Ahsh(N);
		for (int i = 0; i < N; i++) {
			for (int l = 0; l < L; l++) {
				Ahsh[i] += hash_ids[l][A[i][l]];
			}
		}
		std::vector<hsh> Bhsh(Q);
		for (int i = 0; i < Q; i++) {
			for (int l = 0; l < L; l++) {
				Bhsh[i] += hash_ids[l][B[i][l]];
			}
		}

		hash_map<uint64_t, int> mp;
		int64_t ans = 0;
		for (int l = 0; l < L; l++) {
			for (int q = 0; q < Q; q++) {
				for (int z = 0; z < 4; z++) {
					if (z == B[q][l]) continue;
					ans += mp[Bhsh[q] - hash_ids[l][B[q][l]] + hash_ids[l][z]];
				}
			}
			for (int i = 0; i < N; i++) {
				for (int z = 0; z < 4; z++) {
					if (z == A[i][l]) continue;
					mp[Ahsh[i] - hash_ids[l][A[i][l]] + hash_ids[l][z]]++;
				}
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
