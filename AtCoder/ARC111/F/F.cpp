#include <bits/stdc++.h>

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

using num = modnum<998244353>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, Q; cin >> N >> M >> Q;

	// For a cell, all that matters is # of queries, so let's solve it for N =
	// 1 for all Q; we'll later account for probability that it's used.
	//
	// We can also do this trick on values, >= k vs < k;
	// We have k ops of set-0 and M-k ops of set-1 and M no-ops and 1 query;
	// note that P(noop or query) is identical for all values of k, so by
	// linearity of expectation, we can pretend that the operations are "noop v",
	// "set v", and "query". Thus, the value of a query is 0 if there are no
	// previous sets, and (M-1)/2 otherwise.
	//
	// This means that the lth op on a point has EV
	//   (M-1)/2 / (2M+1) * (1 - ((M+1)/(2M+1))^(l-1))
	//
	// If you have exactly l ops, then your value is
	//   (M-1)/2 * (l / (2 * M + 1) - (1 - ((M+1)/(2M+1))^l) / M)
	//
	// The EV of total l for all points is Q * EV range size, which is Q * (N+2) / 3
	//
	// The other term is geometric in l, so we can fold that together with the probabiltiy we're picked.

	num ans = 0;
	num num_ops = num(int64_t(N) * int64_t(N+1) / 2) * num(2*M+1);
	num tot_ways = pow(num_ops, Q);

	ans += tot_ways * num(Q) * num(N+2) / num(3) / num(2*M+1);

	num invM = inv(num(M));
	for (int i = 0; i < N; i++) {
		ans -= (tot_ways - pow(num_ops - num(M) * num(i+1) * num(N-i), Q)) * invM;
	}

	// Every "good" query has this EV
	ans *= num(M-1) / num(2);

	cout << ans << '\n';
	return 0;
}
