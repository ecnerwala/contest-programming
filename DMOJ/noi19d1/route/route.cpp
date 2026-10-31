#include<bits/stdc++.h>

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
	const ll inf = LLONG_MAX;
	ll div(ll a, ll b) { // floored division
		return a / b - ((a ^ b) < 0 && a % b); }
	bool isect(iterator x, iterator y) {
		if (y == end()) { x->p = inf; return false; }
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
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	int64_t A, B, C; cin >> A >> B >> C;
	struct train {
		int x, y; int64_t p, q;
	};
	vector<train> trains(M);
	for (auto& t : trains) {
		cin >> t.x >> t.y >> t.p >> t.q; t.x--, t.y--;
	}

	sort(trains.begin(), trains.end(), [](const train& a, const train& b) { return a.p < b.p; });

	vector<kactl::LineContainer> dp(N);
	int64_t ans = 1e18;

	priority_queue<tuple<int64_t, int, int64_t, int64_t>> to_insert;
	to_insert.emplace(-0, 0, -B, -C);
	for (const auto& t : trains) {
		while (!to_insert.empty() && -get<0>(to_insert.top()) <= t.p) {
			auto [_, y, k, m] = to_insert.top();
			dp[y].add(k, m);
			to_insert.pop();
		}
		if (dp[t.x].empty()) continue;
		int64_t cost = -dp[t.x].query(t.p) + A * t.p * t.p;
		if (t.y == N-1) {
			ans = min(ans, cost + t.q);
			continue;
		}
		// A * (x - t.q)^2 + B * (x - t.q) + C + cost
		to_insert.emplace(-t.q, t.y, -(-2 * A * t.q + B), -(A * t.q * t.q - B * t.q + C + cost));
	}

	cout << ans << '\n';

	return 0;
}
