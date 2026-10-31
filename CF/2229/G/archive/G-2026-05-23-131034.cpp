#include <bits/stdc++.h>
#include <cassert>

namespace kactl {
using namespace std;
using ll = int64_t;

struct Line {
	mutable ll k, m, p;
	bool operator<(const Line& o) const { return k < o.k; }
	bool operator<(ll x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
	// (for doubles, use inf = 1/.0, div(a,b) = a/b)
	static const ll inf = LLONG_MAX;
	ll div(ll a, ll b) { // floored division
		return a / b - ((a ^ b) < 0 && a % b); }
	bool isect(iterator x, iterator y) {
		if (y == end()) return x->p = inf, 0;
		if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
		else x->p = div(y->m - x->m, x->k - y->k);
		return x->p >= y->p;
	}
	void add(ll k, ll m) {
		auto z = insert({k, m, 0}), y = z++, x = y;
		while (isect(y, z)) z = erase(z);
		if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
		while ((y = x) != begin() && (--x)->p >= y->p)
			isect(x, erase(y));
	}
	ll query(ll x) {
		assert(!empty());
		auto l = *lower_bound(x);
		return l.k * x + l.m;
	}
};
}

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K, X; std::cin >> N >> K >> X; X--;
		//std::cerr << "case " << N << ' ' << K << ' ' << X << '\n';
		std::vector<int> V(N); for (auto& h : V) std::cin >> h;
		std::vector<int> road_D(N-1); for (auto& d : road_D) std::cin >> d;
		std::vector<int> D(N);
		D[X] = 0;
		for (int i = X+1; i < N; i++) {
			D[i] = std::max(D[i-1] + 1, road_D[i-1]);
		}
		for (int i = X-1; i >= 0; i--) {
			D[i] = std::max(D[i+1] + 1, road_D[i]);
		}

		// we just go to bigger houses over time, and we always reach them at their opening time
		struct house_t {
			int x;
			int v;
			int d;
			int64_t pref;
			int64_t val = 0; // value of the house is v * time + val in absolute time
		};
		std::array<std::vector<house_t>, 2> H;
		H[0].reserve(N);
		H[1].reserve(N);
		for (int z = 0; z < 2; z++) {
			int64_t pref = 0;
			for (int i = X; i < N && i >= 0; i += (z ? +1 : -1)) {
				// pref is inclusive of everything
				pref += V[i];
				if (!H[z].empty() && H[z].back().v > V[i]) continue;
				H[z].push_back({std::abs(i - X), V[i], D[i], pref});
			}
		}

		// process the houses in increasing order of D
		int64_t ans = int64_t(V[X]) * int64_t(K);

		std::array<int, 2> proc_it{1, 1};
		std::array<int, 2> ins_it{0, 0};
		while (proc_it[0] < int(H[0].size()) || proc_it[1] < int(H[1].size())) {
			int z = proc_it[1] < int(H[1].size()) && (proc_it[0] == int(H[0].size()) || H[1][proc_it[1]].v < H[0][proc_it[0]].v);
			int i = proc_it[z]++;
			auto& h = H[z][i];

			//std::cerr << "checking house " << z << ' ' << h.x << ' ' << h.v << ' ' << h.d << '\n';
			{
				auto& o = H[z][i-1];
				h.val = o.val + int64_t(o.v) * int64_t(h.d - (h.x - o.x)) + (h.pref - o.pref) - int64_t(h.d) * int64_t(h.v);
			}

			while (ins_it[!z] < proc_it[!z] && H[!z][ins_it[!z]].v <= h.v) {
				int j = ins_it[!z]++;
				auto& o = H[!z][j];
				if (o.d + h.x + o.x <= h.d) {
					h.val = std::max(h.val, o.val + int64_t(o.v) * int64_t(h.d - (h.x + o.x - 0)) + o.pref + h.pref - V[X]);
				}
			}

			//std::cerr << "value " << h.val << ' ' << h.val + h.d * int64_t(h.v) << '\n';
			if (h.d <= K) {
				ans = std::max(ans, h.val + int64_t(h.v) * int64_t(K));
			}
		}
		std::cout << ans << '\n';
	}

	return 0;
}
