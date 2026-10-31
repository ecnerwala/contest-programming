#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
template <typename T> int sz(T&& a) { return (int)a.size(); }
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

/**
 * Author: Simon Lindholm
 * Date: 2017-04-20
 * License: CC0
 * Source: own work
 * Description: Container where you can add lines of the form kx+m, and query maximum values at points x.
 *  Useful for dynamic programming (``convex hull trick'').
 * Time: O(\log N)
 * Status: stress-tested
 */

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

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int P, K; cin >> P >> K;

	std::vector<int> A_K(K);
	std::vector<int> B_K(K);

	int V = 0;
	std::vector<int> A;
	std::vector<int> B;
	std::vector<int> X;
	std::vector<int> W;

	std::vector<std::vector<std::pair<int, int>>> by_loc(P);

	std::vector<kactl::LineContainer> containers(P);

	for (int k = 0; k < K; k++) {
		int l; cin >> l;
		for (int i = 0; i < l; i++) {
			if (i) {
				int w; cin >> w;
				W.push_back(w);
			} else {
				W.push_back(-1);
			}
			int x; cin >> x; x--;
			int v = V++;
			X.push_back(x);
			A.push_back(A_K[k]);
			B.push_back(B_K[k]);
			by_loc[x].push_back({B_K[k], v});
		}
	}

	std::vector<int64_t> dist(V, -1);

	for (int p = 0; p < P; p++) {
		std::sort(by_loc[p].begin(), by_loc[p].end(), [&](auto a, auto b) { return a.first > b.first; });
	}

	min_pq<std::pair<int64_t, int>> pq;

	auto try_cnd = [&](int v, int64_t cnd) -> void {
		if (dist[v] == -1 || cnd < dist[v]) {
			pq.push({dist[v] = cnd, v});
		}
	};

	auto insert_line = [&](int p, int64_t t, int a) -> void {
		containers[p].add(-a, -t);
		if (by_loc[p].empty()) return;
		auto [b, v] = by_loc[p].back();
		try_cnd(v, containers[p].query(b));
	};
	insert_line(0, 0, 0);

	std::vector<bool> vis(V);

	std::vector<int64_t> best_dist(P, -1);

	while (!pq.empty()) {
		auto [d, v] = pq.top(); pq.pop();
		assert(dist[v] != -1 && dist[v] >= d);
		if (vis[v]) continue;
		vis[v] = true;
		int x = X[v];
		if (best_dist[x] == -1) {
			best_dist[x] = d;
		}
		while (!by_loc[x].empty() && vis[by_loc[x].back().second]) by_loc[x].pop_back();
		insert_line(x, d, A[v]);
		if (v+1 != V && W[v+1] != -1) {
			try_cnd(v+1, d + W[v+1]);
		}
		if (v != 0 && W[v] != -1) {
			try_cnd(v-1, d + W[v]);
		}
	}

	for (int p = 1; p < P; p++) {
		cout << best_dist[p] << " \n"[p+1==P];
	}

	return 0;
}
