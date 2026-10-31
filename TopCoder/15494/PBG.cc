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

const int MAXG = 2010;
const int MAXP = 2010;
const int MAXB = 2010;
num probGP[MAXG][MAXP];
num probGPSum[MAXG+MAXP];
num probGPB[MAXG+MAXP][MAXB];

const int MAXV = MAXG + MAXP + MAXB;
num fact[MAXV];
num ifact[MAXV];
num inv[MAXV];
void precomp() {
	fact[0] = 1;
	for (int i = 1; i < MAXV; i++) {
		fact[i] = fact[i-1] * num(i);
	}
	ifact[MAXV-1] = fact[MAXV-1].inv();
	for (int i = MAXV-1; i >= 1; i--) {
		ifact[i-1] = ifact[i] * num(i);
	}

	for (int i = 1; i < MAXV; i++) {
		inv[i] = ifact[i] * fact[i-1];
	}
}

class PBG {

public:

	int findEV(int P, int B, int G) {
		precomp();

		for (int i = 0; i <= G; i++) {
			for (int j = 0; j <= P; j++) {
				probGP[i][j] = 0;
			}
		}
		for (int i = 0; i <= G+P; i++) {
			for (int j = 0; j <= B; j++) {
				probGPB[i][j] = 0;
			}
		}

		for (int i = 0; i <= G+P; i++) {
			probGPSum[i] = 0;
		}

		probGP[G][P] = 1;
		for (int i = G; i >= 0; i--) {
			for (int j = P; j >= 1; j--) {
				probGPSum[i+j] += probGP[i][j];

				if (i + j < 2) {
					// no more moves
					continue;
				}
				num curProb = probGP[i][j] * (inv[i+j] * inv[i+j-1] * num(2));
				if (i >= 1) {
					// both are grizzly
					probGP[i-1][j] += curProb * (num(i) * num(i-1) * inv[2]);
				}
				if (j >= 2) {
					// grizzly/polar that's not you, 2 polars that aren't you, or you and a polar w/ 50%
					probGP[i][j-1] += curProb * (num(i) * num(j-1) + num(j-1) * num(j-2) * inv[2] + num(j-1) * inv[2]);
				}
			}
		}

		//for (int i = 1; i <= G+P; i++) { cerr << probGPSum[i] << '\n'; }

		num ans = 0;
		probGPB[G+P][B] = 1;
		for (int i = G+P; i >= 1; i--) {
			for (int j = B; j >= 0; j--) {
				ans += probGPB[i][j] * probGPSum[i];
				if (i + j < 2) {
					// no more moves
					continue;
				}
				num curProb = probGPB[i][j] * (inv[i+j] * inv[i+j-1] * num(2));
				if (i >= 2) {
					probGPB[i-1][j] += curProb * (num(i) * num(i-1) * inv[2]);
				}
				if (j >= 1) {
					probGPB[i][j-1] += curProb * (num(i) * num(j) + num(j) * num(j-1) * inv[2]);
				}
			}
		}
		ans = num(G+P+B+1) - ans;

		return int(ans);
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::PBG;

// vim:ft=cpp
