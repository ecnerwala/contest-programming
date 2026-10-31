#include<bits/stdc++.h>

namespace kactl {
typedef unsigned long long ull;
typedef __uint128_t L;
struct FastMod {
	ull b, m;
	FastMod(ull b) : b(b), m(ull((L(1) << 64) / b)) {}
	ull reduce(ull a) {
		ull q = (ull)((L(m) * a) >> 64);
		ull r = a - q * b; // can be proven that 0 <= r < 2*b
		return r >= b ? r - b : r;
	}
};
}

int MOD;
kactl::FastMod F(2);

template <int& MOD_, kactl::FastMod& F> struct modnum {
	static constexpr int& MOD = MOD_;

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
		v = int(F.reduce(ll(v) * ll(o.v)));
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


using num = modnum<MOD, F>;

num go(int R, int C) {
	using namespace std;
	if (R < C) swap(R, C);
	assert(R >= C);

	const int L = max(R, C);

	const int V = min(MOD-1, L + L);
	vector<num> fact(V+1);
	fact[0] = 1;
	for (int i = 1; i <= V; i++) fact[i] = fact[i-1] * num(i);
	vector<num> ifact(V+1);
	ifact[V] = inv(fact[V]);
	for (int i = V; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);
	vector<num> invs(V+1);
	for (int i = 1; i <= V; i++) invs[i] = fact[i-1] * ifact[i];

	vector<vector<array<array<num, 3>, 2>>> ev(L+1, vector<array<array<num, 3>, 2>>(L+1));
	auto solve_ones = [&](int n, int m) -> void {
		assert(n >= 1 && m >= 1);

		ev[n][m][1][0] = 1 + invs[m] * ev[m][n-1][1][0] + (1 - invs[m]) * ev[m][n-1][0][0];
		ev[n][m][1][1] = 1 + invs[m] * ev[m-1][n-1][0][0] + (1 - invs[m]) * ev[m][n-1][0][0];
		if (m >= 2) {
			ev[n][m][1][2] = 1 + invs[m] * ev[m][n-1][1][0] + invs[m] * ev[m-1][n-1][0][0] + (1 - 2 * invs[m]) * ev[m][n-1][0][0];
		}
	};
	auto solve_start = [&](int n, int m) -> void {
		ev[n][m][0][0] = 1 + invs[m] * ev[m][n][1][n == 1 ? 1 : 2] + (1 - invs[m]) * ev[m][n][0][n == 1 ? 1 : 2];
	};
	auto solve = [&](int n, int m) -> void {
		if (n == 0 || m == 0) {
			for (int a = 0; a < 2; a++) {
				for (int b = 0; b < 3; b++) {
					ev[n][m][a][b] = 0;
					ev[m][n][a][b] = 0;
				}
			}
			return;
		}

		assert(n >= 1 && m >= 1);

		solve_ones(n, m);
		solve_ones(m, n);

		// we now have 4 equations with ev[n][m][0][min(m, 2)], ev[m][n][0][min(n, 2)]
		// ev[n][m][0][1] = 1 + invs[m] * ev[m-1][n][0][n == 1 ? 1 : 2] + (1 - invs[m]) * ev[m][n][0][n == 1 ? 1 : 2]
		// ev[n][m][0][2] = 1 + invs[m] * ev[m-1][n][0][n == 1 ? 1 : 2] + invs[m] * ev[m][n][1][n == 1 ? 1 : 2] + (1 - 2 * invs[m]) * ev[m][n][0][n == 1 ? 1 : 2]

		// x = ev[n][m][0][m == 1 ? 1 : 2], y = ev[m][n][0][n == 1 ? 1 : 2]
		// x = ax * y + bx
		num ax, bx;
		if (m == 1) {
			bx = 1 + invs[m] * ev[m-1][n][0][n == 1 ? 1 : 2];
			ax = (1 - invs[m]);
		} else {
			bx = 1 + invs[m] * ev[m-1][n][0][n == 1 ? 1 : 2] + invs[m] * ev[m][n][1][n == 1 ? 1 : 2];
			ax = (1 - 2 * invs[m]);
		}
		num ay, by;
		if (n == 1) {
			by = 1 + invs[n] * ev[n-1][m][0][m == 1 ? 1 : 2];
			ay = (1 - invs[n]);
		} else {
			by = 1 + invs[n] * ev[n-1][m][0][m == 1 ? 1 : 2] + invs[n] * ev[n][m][1][m == 1 ? 1 : 2];
			ay = (1 - 2 * invs[n]);
		}
		num x, y;
		// num denom = inv(1 - ax * ay);
		num denom;
		if (m == 1 || n == 1) {
			denom = 1;
		} else {
			if (m + n - 2 > V) {
				denom = 0;
			} else {
				denom = invs[m + n - 2] * invs[2] * m * n;
			}
		}
		x = (bx + ax * by) * denom;
		y = (by + ay * bx) * denom;

		ev[n][m][0][1] = 1 + invs[m] * ev[m-1][n][0][n == 1 ? 1 : 2] + (1 - invs[m]) * y;
		if (m >= 2) ev[n][m][0][2] = 1 + invs[m] * ev[m-1][n][0][n == 1 ? 1 : 2] + invs[m] * ev[m][n][1][n == 1 ? 1 : 2] + (1 - 2 * invs[m]) * y;

		ev[m][n][0][1] = 1 + invs[n] * ev[n-1][m][0][m == 1 ? 1 : 2] + (1 - invs[n]) * x;
		if (n >= 2) ev[m][n][0][2] = 1 + invs[n] * ev[n-1][m][0][m == 1 ? 1 : 2] + invs[n] * ev[n][m][1][m == 1 ? 1 : 2] + (1 - 2 * invs[n]) * x;

		assert(ev[n][m][0][m == 1 ? 1 : 2] == x);
		assert(ev[m][n][0][n == 1 ? 1 : 2] == y);

		solve_start(n, m);
		solve_start(m, n);
	};

	for (int i = 0; i <= L; i++) {
		for (int j = 0; j <= i; j++) {
			solve(i, j);
		}
	}

	return ev[R][C][0][0];
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int R, C; cin >> R >> C >> MOD;
	F = kactl::FastMod(MOD);

	cout << go(R, C) << '\n';

	return 0;
}
