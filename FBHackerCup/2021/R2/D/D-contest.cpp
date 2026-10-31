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

int64_t sum[1 << 23];

auto solve(std::vector<std::pair<int64_t, int>> A, int K) -> std::array<std::vector<int>, 2> {
	int N = int(A.size());
	assert(K <= N-2);
	assert(K >= std::min(25, N-2));

	std::array<std::vector<int>, 2> resp;

	if (true) {
		// If we can make anything, we can always solve the thing, so just run some greedy
		std::sort(A.begin(), A.end());
		std::reverse(A.begin(), A.end());
		hash_map<int64_t, std::array<int, 2>> diffs;
		std::vector<std::pair<int64_t, int>> NA; NA.reserve(A.size());
		for (int i = 0; i < N; i += 2) {
			if (i == N-1) {
				NA.push_back(A[i]);
				break;
			}
			int64_t d = A[i].first - A[i+1].first;
			auto [it, did_insert] = diffs.insert({d, {i, i+1}});
			if (!did_insert) {
				//std::cerr << "erasing " << it->second[0] << ' ' << it->second[1] << ' ' << i+1 << ' ' << i << '\n';
				resp[0].push_back(A[it->second[0]].second);
				resp[1].push_back(A[it->second[1]].second);
				resp[0].push_back(A[i+1].second);
				resp[1].push_back(A[i].second);
				diffs.erase(it->first);
			}
		}
		for (auto [_, v] : diffs) {
			NA.push_back(A[v[0]]);
			NA.push_back(A[v[1]]);
		}
		A = NA;
	}

	std::sort(A.begin(), A.end());
	std::reverse(A.begin(), A.end());

	std::vector<std::pair<int64_t, int>> cnds;
	hash_map<int64_t, int> mem;
	sum[0] = 0;
	while (!A.empty()) {
		int i = int(cnds.size());
		cnds.push_back(A.back());
		A.pop_back();

		for (int m = (1 << i); m < (2 << i); m++) {
			sum[m] = sum[m - (1 << i)] + cnds[i].first;
			auto [it, did_insert] = mem.insert({sum[m], m});
			if (!did_insert) {
				int m2 = it->second;
				assert(!(m & m2));
				for (int x = i; x >= 0; x--) {
					if (m & (1 << x)) {
						resp[0].push_back(cnds[x].second);
					} else if (m2 & (1 << x)) {
						resp[1].push_back(cnds[x].second);
					} else {
						// toss it back into the pool
						A.push_back(cnds[x]);
					}
				}
				cnds = {};
				mem = {};
				goto done;
			}
		}
done:;
	}

	return resp;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << '\n';

		int N, K; cin >> N >> K;
		std::vector<std::pair<int64_t, int>> L(N);
		for (int i = 0; i < N; i++) {
			cin >> L[i].first;
			L[i].second = i;
		}

		auto ans = solve(L, K);

		cout << "Case #" << case_num << ": ";
		if (!ans[0].empty()) {
			cout << "Possible" << '\n';
			for (const auto& r : ans) {
				assert(!r.empty());
				for (int z = 0; z < int(r.size()); z++) {
					cout << r[z]+1 << " \n"[z+1==int(r.size())];
				}
			}
		} else {
			cout << "Impossible" << '\n';
		}
	}

	return 0;
}
