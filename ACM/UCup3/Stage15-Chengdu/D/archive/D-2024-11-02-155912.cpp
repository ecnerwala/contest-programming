#include <bits/stdc++.h>

using std::cerr;

struct bound_t {
	int v;
};

bound_t do_min(bound_t a, bound_t b) {
	if (a.v > b.v) std::swap(a.v, b.v);
	assert(a.v <= b.v);
	if (b.v == (a.v | 1)) {
		assert((a.v & 3) == 2);
		assert((b.v & 3) == 3);
		return bound_t{a.v - 2};
	} else {
		return a;
	}
}

bound_t do_max(bound_t a, bound_t b) {
	if (a.v > b.v) std::swap(a.v, b.v);
	assert(a.v <= b.v);
	if (b.v == (a.v | 1)) {
		assert((a.v & 3) == 2);
		assert((b.v & 3) == 3);
		return bound_t{a.v + 2};
	} else {
		return a;
	}
}

struct range_t {
	bound_t lo, hi;

	int sz() const {
		if (lo.v > hi.v) {
			return 0;
		}
		assert(lo.v <= hi.v);
		if ((lo.v >> 1) == (hi.v >> 1)) {
			if ((lo.v & 2) && lo.v == hi.v) {
				return 1;
			} else {
				return 0;
			}
		}
		assert((lo.v >> 1) < (hi.v >> 1));
		return (hi.v >> 1) - (lo.v >> 1);
	}
};

std::optional<std::vector<int>> solve(std::vector<int> P, int K) {
	int N = int(P.size());
	assert(N >= 2);
	if (!(N & 1)) {
		// N even
		if (K == 1) {
			std::vector<int> Q(N);
			for (int i = 0; i < N; i++) Q[i] = P[i] ^ 1;
			return Q;
		} else {
			return std::nullopt;
		}
	}
	assert(N & 1);
	assert(N >= 3);
	assert(K >= 1);
	int L = (N-1) / 2;
	// middle odd number
	// there are 3 * L + 1 different possible endpoints
	// we'll just store them as 4 * l + 0, 4 * l + 2, 4 * l + 3
	range_t cur{{0}, {4*L}};
	assert(cur.sz() == 2*L);
	if (K > cur.sz()) return std::nullopt;

	std::vector<int> Q(N);
	std::vector<std::pair<int, range_t>> cnds; cnds.reserve(4);
	for (int i = 0; i < N; i++) {
		int p = P[i];
		cerr << "process " << p << '\n';
		cnds.clear();
		if (p & 1) {
			// we can go up or down
			// p*2 + 0 means (+1, -1) +2, -1, -1 (+1 -1)
			// p*2 + 1 means (+1, -1) +1, +1, -2 (+1 -1)
			cnds.push_back({-1, {{p * 2 + 0}, {4*L}}});
			cnds.push_back({+1, {{0*L}, {p * 2 + 1}}});
		} else {
			cnds.push_back({-2, {{(p-1) * 2 + 1}, {(p-1) * 2 + 1}}});
			cnds.push_back({-1, {{4*0}, {(p-1) * 2 + 0}}});
			cnds.push_back({+1, {{(p+1) * 2 + 1}, {4*L}}});
			cnds.push_back({+2, {{(p+1) * 2 + 0}, {(p+1) * 2 + 0}}});
		}
		int tot_sz = 0;
		for (auto& [delta, rng] : cnds) {
			cerr << ' ' << delta << ' ' << rng.lo.v << '-' << rng.hi.v << '\n';
			rng.lo = do_max(rng.lo, cur.lo);
			rng.hi = do_min(rng.hi, cur.hi);
			cerr << ' ' << "  " << rng.lo.v << '-' << rng.hi.v << '\n';
			tot_sz += rng.sz();
		}
		assert(tot_sz == cur.sz());
		for (int idx = 0; true; idx++) {
			assert(idx < int(cnds.size()));
			auto [delta, rng] = cnds[i];
			assert(1 <= K);
			if (K <= rng.sz()) {
				Q[i] = P[i] + delta;
				cur = rng;
				break;
			} else {
				K -= rng.sz();
			}
		}
	}
	return Q;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> P(N);
		for (auto& p : P) { cin >> p; p--; }
		auto res = solve(P, K);
		if (res) {
			for (int i = 0; i < N; i++) {
				cout << (*res)[i] + 1 << " \n"[i+1==N];
			}
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
