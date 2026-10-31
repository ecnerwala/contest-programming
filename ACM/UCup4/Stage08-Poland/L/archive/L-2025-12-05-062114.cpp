#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef pair<int, int> pii;
typedef vector<int> vi;

using dbl = double;

struct Line {
	mutable dbl k, m, p;
	bool operator<(const Line& o) const { return k < o.k; }
	bool operator<(dbl x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
	// (for doubles, use inf = 1/.0, div(a,b) = a/b)
	static constexpr dbl inf = 1e299;
	dbl div(dbl a, dbl b) { // floored division
		return a / b; }
	bool isect(iterator x, iterator y) {
		if (y == end()) return x->p = inf, 0;
		if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
		else x->p = div(y->m - x->m, x->k - y->k);
		return x->p >= y->p;
	}
	void add(dbl k, dbl m) {
		auto z = insert({k, m, 0}), y = z++, x = y;
		while (isect(y, z)) z = erase(z);
		if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
		while ((y = x) != begin() && (--x)->p >= y->p)
			isect(x, erase(y));
	}
	dbl query(dbl x) {
		assert(!empty());
		auto l = *lower_bound(x);
		return l.k * x + l.m;
	}
};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, K; cin >> N >> M >> K;
	std::vector<int> A(N);
	for (auto& a : A) { cin >> a; a--; }

	if (K == 1) {
		std::vector<bool> has(M);
		for (auto a : A) has[a] = true;
		for (int j = 0; j < M; j++) {
			cout << (has[j] ? 0 : -1) << '\n';
		}
	} else {
		assert(K >= 2);
		std::vector<double> ans(M, 1);
		for (int z = 0; z < 2; z++) {
			for (auto& a : A) a = M-1-a;
			std::reverse(ans.begin(), ans.end());

			int mi = *std::min_element(A.begin(), A.end());
			int ma = *std::max_element(A.begin(), A.end());
			std::vector<int64_t> best(M, -1);

			std::vector<int64_t> pref(N+1);
			for (int i = 0; i < N; i++) { pref[i+1] = pref[i] + A[i]; }

			std::vector<std::pair<int64_t, int>> stk; stk.reserve(N);
			int lo = 0;
			for (int i = 0; i < N; i++) {
				if (lo < int(stk.size()) && stk[lo].second == i) lo++;
				if (i+K <= N) {
					int64_t cnd = pref[i+K] - pref[i];
					while (lo < int(stk.size()) && cnd >= stk.back().first) stk.pop_back();
					stk.push_back({cnd, i+K});
				}
				assert(lo < int(stk.size()));
				best[A[i]] = std::max(best[A[i]], stk.back().first);
			}
			kactl::LineContainer lc;
			lc.add(0, -1);
			for (int j = mi; j <= ma; j++) {
				if (best[j] > int64_t(j) * K) {
					int64_t delta = best[j] - int64_t(j) * K;
					// f(j) = 0, f(j + delta/K) = -1
					// slope = K / delta
					double slope = -double(K) / double(delta);
					lc.add(slope, -slope * j);
				}
				ans[j] = std::min(ans[j], -lc.query(j));
			}
		}

		{
			int mi = *std::min_element(A.begin(), A.end());
			int ma = *std::max_element(A.begin(), A.end());
			cout << fixed << setprecision(12);
			for (int j = 0; j < M; j++) {
				if (mi <= j && j <= ma) {
					ans[j] = std::max(ans[j], 0.);
					ans[j] = std::min(ans[j], 1.);
					cout << ans[j] << '\n';
				} else cout << -1 << '\n';
			}
		}
	}

	return 0;
}
