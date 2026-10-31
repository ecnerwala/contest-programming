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

class DiagonalColumn {

public:

	int countGrids(int H, int W) {
		map<pair<int, int>, num> dp; // num known, num known 1
		dp[{0, 0}] += num(1);

		for (int col = 0; col < W; col++) {
			map<pair<int, int>, num> ndp;
			for (auto it : dp) {
				const int nk = it.first.first;
				const int nk1 = it.first.second;
				const num ways = it.second;
				//cerr << col << ' ' << nk << ' '<< nk1 << ' ' << ways << '\n';
				{
					// we take the col, in which case nothing gets marked as known
					// lowest order one dies, if it wasn't known, it doesn't matter
					ndp[{nk, nk1}] += ways;
				}
				{
					// we don't take the col, so everything gets marked as known
					for (int i = 0; i <= H - nk; i++) {
						if (i == H - nk) {
							if (nk1 < nk) {
								num colWays = 1;
								ndp[{H, i + nk1}] += ways * colWays;
							}
						} else {
							num colWays = num(1ll << (H - nk - i - 1));
							//cerr << i << ' ' << colWays << '\n';
							ndp[{H, i}] += ways * colWays;
						}
					}
				}
			}
			dp = {};
			for (auto it : ndp) {
				int nk = it.first.first;
				int nk1 = it.first.second;
				const num ways = it.second;
				if (nk) {
					nk--;
					nk1 = min(nk1, nk);
				}
				dp[{nk, nk1}] += ways;
			}
		}
		num ans = 0;
		for (auto it : dp) {
			//cerr << W << ' ' << it.first.first << ' ' << it.first.second << ' ' << it.second << '\n';
			ans += it.second;
		}
		return int(ans);
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << DiagonalColumn().countGrids(2, 4) << '\n';
	cerr << DiagonalColumn().countGrids(31, 38) << '\n';
	long long tot = 0;
	for (int h = 1; h <= 38; h++) {
		for (int w = 1; w <= 38; w++) {
			int v = DiagonalColumn().countGrids(h, w);
			tot += v;
			//cerr << h << ' ' << w << ' ' << v << '\n';
		}
	}
	cerr << tot << '\n';
}

}  // namespace ecnerwala

using ecnerwala::DiagonalColumn;

// vim:ft=cpp
