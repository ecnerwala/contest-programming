#include <bits/extc++.h>

void aggiungi();
void copia();
void incolla();

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

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

hash_map<long long, std::pair<long long, int>> cache;
long long solve(long long a) {
	if (a == 0) return 0;
	auto it = cache.find(a);
	if (it != cache.end()) return it->second.first;
	std::pair<long long, int> r = {a, -1};
	for (int d : {2, 3, 4, 5, 7, 11, 13, 17, 19}) {
		std::pair<long long, int> cnd = {solve(a / d) + (a % d) + (d+1), d};
		if (cnd < r) r = cnd;
	}
	//std::cerr << a << ' ' << r.first << ' ' << r.second << '\n';
	cache.insert({a, r});
	return r.first;
}

void do_ops(long long a) {
	if (a == 0) return;

	int d = cache[a].second;
	if (d == -1) {
		for (int i = 0; i < a; i++) {
			aggiungi();
		}
	} else {
		do_ops(a / d);
		copia();
		for (int i = 0; i < d-1; i++) {
			incolla();
		}
		for (int i = 0; i < a % d; i++) {
			aggiungi();
		}
	}
}

void auguri(long long int N) {
	for (int z = 0; z < 1000; z++) {
		solve(z);
	}
	solve(N);
	do_ops(N);
}

// 3 to 2x
// 4 to 3x
// 5 to 4x
// 6 to 5x
// 7 to 6x <- never 6x
// 8 to 7x <- maybe???
// 9 to 8x <- never 8x
// 10 to 9x <- never 9x
// 11 to 10x <- never 10x
// 12 to 11x <- seems unlikely...
