#include<bits/stdc++.h>
using namespace std;

using i64 = int64_t;

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
using num = modnum<998244353>;

struct range {
	i64 v;
	int l, r;

	range(): v(0), l(-1), r(-1) {}
	range(i64 v_, int l_, int r_) : v(v_), l(l_), r(r_) {}
	range(const range& a, const range& b) {
		assert(a.r == b.l);
		v = a.v + b.v;
		l = a.l;
		r = b.r;
	}
	friend bool operator < (const range& a, const range& b) { return a.v < b.v; }
};

struct seg_node {
	range wholeRange;
	range maxPref, maxSuff, maxRange;
	range minPref, minSuff, minRange;

	seg_node() {}
	seg_node(int ind, i64 v) {
		wholeRange = maxPref = maxSuff = maxRange = minPref = minSuff = minRange = range(v, ind, ind+1);
	}

	seg_node(const seg_node& a, const seg_node& b) {
		if (a.wholeRange.l == -1) {
			*this = b;
			return;
		}
		if (b.wholeRange.l == -1) {
			*this = a;
			return;
		}
		wholeRange = {a.wholeRange, b.wholeRange};
		maxPref = max(a.maxPref, range(a.wholeRange, b.maxPref));
		maxSuff = max(range(a.maxSuff, b.wholeRange), b.maxSuff);
		maxRange = max(max(a.maxRange, b.maxRange), range(a.maxSuff, b.maxPref));

		minPref = min(a.minPref, range(a.wholeRange, b.minPref));
		minSuff = min(range(a.minSuff, b.wholeRange), b.minSuff);
		minRange = min(min(a.minRange, b.minRange), range(a.minSuff, b.minPref));
	}
};

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N; cin >> N;
	vector<i64> A(N);
	for (int i = 0; i < N; i++) cin >> A[i];

	const int S = 1 << 17;
	vector<seg_node> seg(2*S);
	for (int i = 0; i < N; i++) {
		seg[S+i] = seg_node(i, A[i]);
	}
	for (int i = S-1; i; i--) seg[i] = seg_node(seg[2*i], seg[2*i+1]);

	auto queryRange = [&](int l, int r) {
		assert(l <= r);
		seg_node lval;
		seg_node rval;
		for (int a = S+l, b = S+r+1; a < b; a /= 2, b /= 2) {
			if (a & 1) lval = seg_node(lval, seg[a++]);
			if (b & 1) rval = seg_node(seg[--b], rval);
		}
		return seg_node(lval, rval);
	};

	//cerr << queryRange(0, N-1).maxRange.v << '\n';

	priority_queue<range> pq;
	// out followed by in
	auto addOutCnd = [&](int l, int r) {
		assert(l <= r);
		if (l == r) return;
		range cnd = queryRange(l, r-1).maxRange;
		assert(cnd.l < cnd.r);
		pq.push(cnd);
	};
	// in followed by out
	auto addInCnd = [&](int l, int r) {
		assert(l < r);
		if (r - l == 1) return;
		range cnd(0, l+1, l+1);
		if (r - l > 2) {
			range c2 = queryRange(l+1, r-2).minRange;
			c2.v *= -1;
			swap(c2.l, c2.r);
			cnd = max(cnd, c2);
		}
		assert(cnd.l >= cnd.r);
		pq.push(cnd);
	};

	auto addCnd = [&](pair<int, bool> p, pair<int, bool> q) {
		assert(p.second != q.second);
		if (p.second == false) {
			addOutCnd(p.first, q.first);
		} else {
			addInCnd(p.first, q.first);
		}
	};

	// out is false, in is true
	set<pair<int, bool>> cuts;
	cuts.emplace(0, false);
	cuts.emplace(N, true);
	addOutCnd(0, N);

	vector<i64> flows; flows.reserve(N);
	for (int i = 0; i < N; i++) {
		range cur = pq.top(); pq.pop();
		flows.push_back(cur.v);
		pair<int, bool> a, b;
		if (cur.l < cur.r) {
			a = {cur.l, true};
			b = {cur.r, false};
		} else {
			a = {cur.r, false};
			b = {cur.l, true};
		}
		auto it = cuts.insert(a).first;
		--it;
		auto jt = cuts.insert(b).first;
		++jt;

		addCnd(*it, a);
		addCnd(a, b);
		addCnd(b, *jt);
	}

	i64 K; cin >> K; K++;
	num ans = 0;
	for (int rep = 1; rep <= N; rep++) {
		i64 multiplicity = K;
		if (rep < N) {
			multiplicity = min(multiplicity, (flows[rep-1] - flows[rep]) * rep);
		}
		K -= multiplicity;

		i64 curVal = flows[rep-1];
		i64 numWhole = multiplicity / rep;
		i64 numPart = multiplicity % rep;

		//cerr << rep << ' ' << curVal << ' ' << multiplicity << '\n';

		ans += num(curVal * 2 - numWhole + 1) * num(numWhole * rep) / num(2);
		ans += num(curVal - numWhole) * num(numPart);
	}

	ans -= flows[0];

	cout << ans << '\n';

	return 0;
}
