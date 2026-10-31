#include<bits/stdc++.h>

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = minv(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		if (v < 0) v += MOD;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(ll(v) * ll(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

// https://github.com/kth-competitive-programming/kactl/blob/master/content/data-structures/LineContainer.h

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

#undef sz
#undef all
#undef rep
} // namespace kactl

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, Q, K; cin >> N >> Q >> K;

		auto read_vec_mod_i = [K](int len) -> vector<int64_t> {
			vector<int64_t> res(len);
			for (int i = 0; i < K; i++) {
				cin >> res[i];
			}
			int64_t a, b, c; cin >> a >> b >> c;
			for (int i = K; i < len; i++) {
				res[i] = (a * res[i-2] + b * res[i-1] + c) % i + 1;
			}
			return res;
		};

		auto read_vec_with_mod = [K](int len, int64_t mod) -> vector<int64_t> {
			vector<int64_t> res(len);
			for (int i = 0; i < K; i++) {
				cin >> res[i];
			}
			int64_t a, b, c; cin >> a >> b >> c;
			for (int i = K; i < len; i++) {
				res[i] = (a * res[i-2] + b * res[i-1] + c) % mod + 1;
			}
			return res;
		};

		auto read_vec_with_d = [K](int len) -> vector<int64_t> {
			vector<int64_t> res(len);
			for (int i = 0; i < K; i++) {
				cin >> res[i];
			}
			int64_t a, b, c, d; cin >> a >> b >> c >> d;
			for (int i = K; i < len; i++) {
				res[i] = (a * res[i-2] + b * res[i-1] + c) % d + 1;
			}
			return res;
		};

		vector<int64_t> P = read_vec_mod_i(N); for (auto& v : P) v--;
		vector<int64_t> L = read_vec_with_d(N);
		vector<int64_t> H = read_vec_with_d(N);
		vector<int64_t> X = read_vec_with_mod(Q, N); for (auto& v : X) v--;
		vector<int64_t> Y = read_vec_with_d(Q);

		assert(P[0] == -1);

		// make this the prefix depth
		for (int i = 1; i < N; i++) {
			L[i] += L[P[i]];
		}

		// this queries max, so we'll negate it
		vector<kactl::LineContainer> subtree_opts(N);
		vector<vector<pair<int64_t, int64_t>>> lines(N);

		vector<vector<int>> queries(N);
		for (int q = 0; q < Q; q++) queries[X[q]].push_back(q);
		vector<int64_t> ans(Q);
		// cost(cur, curH) =
		// min_{descendant nxt}
		//   (depth[nxt] - depth[cur]) * curH + cost(nxt, H[nxt])
		//   (depth[nxt] - depth[cur]) * curH + cost(nxt, H[nxt])

		for (int i = N-1; i >= 0; i--) {
			int64_t cost;
			if (subtree_opts[i].empty()) {
				cost = 0;
			} else {
				cost = -subtree_opts[i].query(H[i]) - L[i] * H[i];
			}
			lines[i].push_back({-L[i], -cost});
			subtree_opts[i].add(-L[i], -cost);

			for (int q : queries[i]) {
				ans[q] = -subtree_opts[i].query(Y[q]) - L[i] * Y[q];
			}

			if (P[i] != -1) {
				if (lines[i].size() > lines[P[i]].size()) {
					swap(subtree_opts[i], subtree_opts[P[i]]);
					swap(lines[i], lines[P[i]]);
				}
				//for (auto it : subtree_opts[i]) { subtree_opts[P[i]].add(it.k, it.m); }
				for (auto it : lines[i]) {
					lines[P[i]].push_back(it);
					subtree_opts[P[i]].add(it.first, it.second);
				}
			}
		}

		using num = modnum<int(1e9)+7>;
		num prod_ans = 1;
		for (int64_t v : ans) prod_ans *= v+1;
		cout << "Case #" << case_num << ": " << prod_ans << '\n';
	}

	return 0;
}
