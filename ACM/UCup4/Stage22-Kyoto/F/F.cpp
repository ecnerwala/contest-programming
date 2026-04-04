#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>

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
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int A, B; std::cin >> A >> B;

	std::vector<int> fac_B; fac_B.reserve(2000);
	for (int i = 1; i * i <= B; i++) {
		if (B % i == 0) {
			fac_B.push_back(i);
			if (i * i < B) fac_B.push_back(B / i);
		}
	}
	const int64_t MX = int64_t(1e16);
	std::vector<std::array<int64_t, 2>> edges; edges.reserve(int(1e7));
	for (int g : fac_B) {
		int l_over_g = A + (B / g);
		for (int i = 1; i * i <= l_over_g; i++) {
			if (l_over_g % i == 0 && std::gcd(i, l_over_g / i) == 1) {
				int64_t x = int64_t(g) * int64_t(i), y = int64_t(g) * int64_t(l_over_g / i);
				assert(x <= y);
				if (y <= MX) {
					// merge x and y
					edges.push_back({x, y});
				}
			}
		}
	}

	hash_map<int64_t, int> ids;
	std::vector<int> par; par.reserve(2 * edges.size());
	std::vector<int64_t> xor_vals; xor_vals.reserve(2 * edges.size());
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		std::cerr << "merge " << a << ' ' << b << '\n';
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		xor_vals[a] ^= xor_vals[b];
		par[b] = a;
		return true;
	};
	for (auto e : edges) {
		for (auto v : e) {
			auto [it, did_insert] = ids.insert({v, int(ids.size())});
			if (did_insert) {
				par.push_back(-1);
				xor_vals.push_back(v);
			}
			v = it->second;
		}
		merge(int(e[0]), int(e[1]));
	}
	auto query = [&](int64_t x) -> int64_t {
		auto it = ids.find(x);
		if (it == ids.end()) return x;
		return xor_vals[get_par(it->second)];
	};

	int Q; std::cin >> Q;
	while (Q--) {
		int64_t x; std::cin >> x;
		std::cout << query(x) << '\n';
	}

	return 0;
}
