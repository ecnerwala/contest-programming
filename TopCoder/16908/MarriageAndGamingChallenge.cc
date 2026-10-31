#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/extc++.h>
#include <chrono>
using namespace std;

namespace ecnerwala {

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = ::std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

class MarriageAndGamingChallenge {

public:

	long long solve(int N, int D, int state, int B, int P, int Q) {
		auto rnd = [&]() -> int {
			state = int((int64_t(state) * 1103515245ll + 12345ll) % (1ll << 31));
			return state;
		};

		vector<int> parent(N);
		vector<int> weight(N);
		parent[0] = -1;
		weight[0] = -1;

		{
			// generate the parents of vertices other than 0
			for (int i = 1; i <= N-1; i++) {
				parent[i] = max( 0, i - 1 - (rnd() % D) );
			}

			// prepare a bank of some edge weights that will appear frequently
			vector<int> bank; bank.reserve(B);
			for (int i = 0; i <= B-1; i++) {
				bank.push_back( rnd() % N );
			}

			// assign edge weights
			for (int i = 1; i <= N-1; i++) {
				if (rnd() % 100 < P) {
					weight[i] = rnd() % N;
				} else {
					weight[i] = bank[ rnd() % B ];
				}
			}
		}

		int S = 1 << 17;

		hash_map<uint64_t, int> mp;
		vector<pair<int, int>> nodes; nodes.reserve(int(1e7));
		nodes.push_back({0,0});
		nodes.push_back({1,1});

		auto get_node = [&](pair<int, int> a) -> int {
			uint64_t key = (uint64_t(a.first) << 32) | uint64_t(a.second);
			auto it = mp.find(key);
			if (it != mp.end()) return it->second;
			int i = int(nodes.size());
			nodes.push_back(a);
			mp.insert({key, i});
			return i;
		};

		int EMPTY_TREE = 0;
		for (int s = 1; s < S; s *= 2) EMPTY_TREE = get_node({EMPTY_TREE, EMPTY_TREE});

		std::function<int(int, int, int, int)> flip = [&](int cur_tree, int x, int lo, int hi) -> int {
			assert(lo <= x && x < hi);
			if (hi - lo == 1) {
				assert(cur_tree == 0 || cur_tree == 1);
				return cur_tree ^ 1;
			}
			assert(cur_tree >= 2);
			int md = (lo + hi) / 2;
			auto cur = nodes[cur_tree];
			if (x < md) {
				cur.first = flip(cur.first, x, lo, md);
			} else {
				cur.second = flip(cur.second, x, md, hi);
			}
			return get_node(cur);
		};

		vector<int> segs(N);
		segs[0] = EMPTY_TREE;
		for (int i = 1; i < N; i++) {
			segs[i] = flip(segs[parent[i]], weight[i], 0, S);
		}

		std::function<int(int, int, int, int, int, int)> check_range = [&](int t1, int t2, int lo, int hi, int l, int r) -> int {
			if (r <= lo || hi <= l) {
				return -1;
			}
			if (t1 == t2) return -1;
			if (hi - lo == 1) {
				return lo;
			}
			int md = (lo + hi) / 2;
			auto c1 = nodes[t1];
			auto c2 = nodes[t2];
			{
				auto x = check_range(c1.second, c2.second, md, hi, l, r);
				if (x != -1) return x;
			}
			{
				auto x = check_range(c1.first, c2.first, lo, md, l, r);
				if (x != -1) return x;
			}
			return -1;
		};

		// returns the index of the first mismatch
		auto query = [&](int u, int v, int hi) -> int {
			int mismatch = check_range(segs[u], segs[v], 0, S, 0, hi+1);
			return hi - mismatch;
		};

		int64_t ans = 0;
		for (int q = 0; q < Q; q++) {
			int u = rnd() % N;
			int v = rnd() % N;
			int hi = rnd() % N; // find the first location that the 2 hashes differ, or -1
			ans += query(u, v, hi);
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::MarriageAndGamingChallenge;

// vim:ft=cpp
