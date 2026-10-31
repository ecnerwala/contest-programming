#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

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
		res.v = mod_inv_in_range(v, MOD);
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
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<998244353>;
	const int V = int(1e6);
	vector<num> fact(V+1);
	fact[0] = 1;
	for (int i = 1; i <= V; i++) fact[i] = fact[i-1] * num(i);
	vector<num> ifact(V+1);
	ifact[V] = inv(fact[V]);
	for (int i = V; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);
	auto choose = [&](int n, int r) {
		return 0 <= r && r <= n ? fact[n] * ifact[r] * ifact[n-r] : num(0);
	};

	// number of <= start, number of > start
	// assumes the end is > start
	auto solve = [&](int s, int b) -> num {
		assert(s >= 1);
		if (b < 2) return 0;
		num ans = 0;
		ans += choose(b + s - 1, b - 2) + num(s-1) * choose(b + s - 2, b - 3);
		/*
		for (int k = 0; k+1 <= b && k <= s; k++) {
			assert(b >= 2);
			ans += choose(b, k+1) * k * choose(s-1, k-1);
		}
		*/
		ans *= fact[b-2];
		ans *= fact[s-1];
		return ans;
	};

	int T; cin >> T;
	while (T--) {
		int N, A; cin >> N >> A;
		cout << fact[N-1] - solve(A, N-A) - solve(N+1-A, A-1) << '\n';
	}

	return 0;
}

// Good iff there is some adjacent small - big
// Let there be s smalls and b meds+bigs (s+b = N)
// First, arrange all smalls in - fact[s-1] ways
// Then, we insert some ranges of bigs (including one at the end)
// Then it's bad iff end > all range-starts
// If there are k >= 1 ranges, we pick the set of range starts/final value in
//   (b choose k+1), then we order them in k * fact[b-2] ways, then we insert them in (s-1 choose k-1) ways
//   sum over all k of (b choose k+1) * k * fact[b-2] * (s-1 choose k-1)
