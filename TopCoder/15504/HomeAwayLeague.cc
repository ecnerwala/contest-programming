#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

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
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
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
using num = modnum<int(1e9)+7>;

class HomeAwayLeague {

public:

	int matches(int N) {
		assert(N % 2 == 0);
		N /= 2;
		// (2N choose N) * N! * !N
		vector<num> factorials(N+1);
		vector<num> derangements(N+1);
		factorials[0] = 1;
		factorials[1] = 1;
		derangements[0] = 1;
		derangements[1] = 0;
		for (int i = 2; i <= N; i++) {
			factorials[i] = factorials[i-1] * num(i);
			derangements[i] = (derangements[i-1] + derangements[i-2]) * num(i-1);
		}
		num ans = factorials[N] * derangements[N];
		for (int i = 1; i <= 2 * N; i++) {
			ans *= num(i);
		}
		ans /= factorials[N];
		ans /= factorials[N];
		return int(ans);
	}
};

void my_tests() {
	cerr << HomeAwayLeague().matches(4) << '\n';
	cerr << HomeAwayLeague().matches(6) << '\n';
	cerr << HomeAwayLeague().matches(8) << '\n';
	cerr << HomeAwayLeague().matches(int(5e5)) << '\n';
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::HomeAwayLeague;

// vim:ft=cpp
