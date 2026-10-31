#include <bits/stdc++.h>

uint64_t bit_reverse(uint64_t a) {
	uint64_t r = 0;
	for (int z = 0; z < 64; z++) {
		r <<= 1;
		r |= (a & 1);
		a >>= 1;
	}
	return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> par(N);
	std::vector<std::vector<int>> ch(N);
	par[0] = -1;
	for (int i = 1; i < N; i++) {
		cin >> par[i]; par[i]--;
		ch[par[i]].push_back(i);
	}

	using dist_basis_t = std::vector<std::pair<int, uint64_t>>;

	auto merge = [&](const dist_basis_t& a, const dist_basis_t& b) -> dist_basis_t {
		dist_basis_t c; c.reserve(std::min(int(a.size()) + int(b.size()), 64));
		for (int ia = 0, ib = 0; ia < int(a.size()) || ib < int(b.size()); ) {
			std::pair<int, uint64_t> entry;
			if (ia < int(a.size()) && (ib == int(b.size()) || a[ia].first <= b[ib].first)) {
				entry = a[ia++];
			} else {
				entry = b[ib++];
			}

			for (auto [_, vec] : c) {
				entry.second = std::min(entry.second, entry.second ^ vec);
			}
			if (entry.second) {
				c.push_back(entry);
			}
		}
		return c;
	};

	auto incr = [&](dist_basis_t a) -> dist_basis_t {
		for (auto& [d, _] : a) d++;
		return a;
	};

	std::vector<dist_basis_t> self_bases(N);

	for (int i = 0; i < N; i++) {
		int M; cin >> M;
		std::vector<uint64_t> basis;
		basis.reserve(std::min(M, 64));
		for (int j = 0; j < M; j++) {
			uint64_t x; cin >> x;
			for (auto b : basis) x = std::min(x, x ^ b);
			if (x) {
				for (auto& b : basis) b = std::min(b, x ^ b);
				basis.push_back(x);
			}
		}
		std::array<uint64_t, 64> basis_vecs; basis_vecs.fill(0);
		for (auto b : basis) {
			basis_vecs[63-__builtin_clzll(b)] = b;
		}
		self_bases[i].reserve(64 - int(basis.size()));
		for (int b = 0; b < 64; b++) {
			if (!basis_vecs[b]) {
				uint64_t eqn = uint64_t(1) << b;
				for (int j = b+1; j < 64; j++) {
					if ((basis_vecs[j] >> b) & 1) {
						eqn |= (uint64_t(1) << j);
					}
				}
				self_bases[i].push_back({0, bit_reverse(eqn)});
			}
		}
		/*
		cerr << i << '\n';
		for (auto [d, v] : self_bases[i]) {
			cerr << d << ' ' << bitset<64>(v) << '\n';
		}
		*/
	}

	std::vector<dist_basis_t> dp(N);

	for (int i = N-1; i >= 0; i--) {
		dp[i] = self_bases[i];
		for (int j : ch[i]) {
			dp[i] = merge(dp[i], incr(dp[j]));
		}
	}
	for (int i = 0; i < N; i++) {
		auto tmp = incr(dp[i]);
		for (int j : ch[i]) {
			dp[j] = merge(dp[j], tmp);
		}
	}

	for (int i = 0; i < N; i++) {
		uint64_t ans = 0;

		/*
		cerr << i << '\n';
		for (auto [d, v] : dp[i]) {
			cerr << d << ' ' << bitset<64>(v) << '\n';
		}
		*/

		std::array<uint64_t, 64> basis_vecs; basis_vecs.fill(0);
		for (int z = 0; z <= int(dp[i].size()); z++) {
			if (z > 0) {
				uint64_t b = dp[i][z-1].second;
				basis_vecs[63-__builtin_clzll(b)] = b;
			}
			int st = (z == 0) ? 0 : dp[i][z-1].first;
			int en = (z == int(dp[i].size())) ? N-1 : dp[i][z].first-1;
			int w = (en - st + 1);
			uint64_t v = 0;
			for (int d = 0; d < 64; d++) {
				if (!basis_vecs[d]) {
					// set it to 1
					v |= uint64_t(1) << d;
				} else {
					v |= uint64_t(__builtin_popcountll(v & basis_vecs[d]) & 1) << d;
				}
			}
			//cerr << st << '-' << en << ' ' << bitset<64>(v) << '\n';
			ans += uint64_t(w) * bit_reverse(v);
		}

		cout << ans << '\n';
	}

	return 0;
}
