#include<bits/stdc++.h>
using namespace std;

namespace kactl {
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
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
} // namespace kactl
using kactl::LineContainer;

using i64 = int64_t;

const int SegS = 1 << 18;
LineContainer seg[2*SegS];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int S, P, D; cin >> S >> P >> D;
	vector<i64> V(P);
	vector<pair<int, int>> probDays(P);
	for (int p = 0; p < P; p++) {
		cin >> V[p] >> probDays[p].first >> probDays[p].second;
	}

	vector<vector<int>> probStudents(P+1);

	vector<int> studentDays(S);
	for (int s = 0; s < S; s++) {
		int a; cin >> a >> studentDays[s];
		probStudents[a].push_back(s);
	}

	vector<i64> ans(S);
	i64 curSuff = 0;
	i64 curPenalty = 0;
	for (int p = P; p >= 0; p--) {
		if (p < P) {
			// add this problem
			curSuff += V[p];
			curPenalty += curSuff;

			i64 k = -V[p], m = V[p] * p + curSuff;
			int l = probDays[p].first, r = probDays[p].second;
			for (int a = SegS+l, b = SegS+r+1; a < b; a /= 2, b /= 2) {
				if (a & 1) seg[a++].add(k, m);
				if (b & 1) seg[--b].add(k, m);
			}
		}

		for (int s : probStudents[p]) {
			i64 bestSave = 0;
			int d = studentDays[s];
			for (int a = SegS+d; a; a /= 2) {
				if (!seg[a].empty()) bestSave = max(bestSave, i64(seg[a].query(p)));
			}
			ans[s] = curPenalty - bestSave;
		}
	}

	for (int s = 0; s < S; s++) {
		cout << ans[s] << '\n';
	}

	return 0;
}
