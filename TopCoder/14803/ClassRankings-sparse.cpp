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

using num = modnum<int(1e9)+7>;

const int MAXN = 51;
const int MAXL = 1010;
num dp[MAXN][MAXN][MAXN][MAXN * 3];

class ClassRankings {

public:

	int countWays(vector <int> amt, vector <int> lo, vector <int> hi) {
		memset(dp, 0, sizeof(dp));

		dp[0][0][0][0] = 1;

		auto getLbase = [&lo](int i, int j, int k) {
			return max({(i ? lo[0] : 0), (j ? lo[1] : 0), (k ? lo[2] : 0)});
		};

		for (int i = 0; i <= amt[0]; i++) {
			for (int j = 0; j <= amt[1]; j++) {
				for (int k = 0; k <= amt[2]; k++) {
					int lBase = getLbase(i,j,k);
					for (int z = 0; z <= (i ? amt[0] : 0) + (j ? amt[1] : 0) + (k ? amt[2] : 0); z++) {
						int l = lBase + z;
						if (i < amt[0] && l+1 <= hi[0]) {
							dp[i+1][j][k][max(l+1, lo[0])-getLbase(i+1,j,k)] += dp[i][j][k][l-lBase];
						}
						if (j < amt[1] && l+1 <= hi[1]) {
							dp[i][j+1][k][max(l+1, lo[1])-getLbase(i,j+1,k)] += dp[i][j][k][l-lBase];
						}
						if (k < amt[2] && l+1 <= hi[2]) {
							dp[i][j][k+1][max(l+1, lo[2])-getLbase(i,j,k+1)] += dp[i][j][k][l-lBase];
						}
					}
				}
			}
		}
		num ans = 0;
		for (int z = 0; z <= amt[0]+amt[1]+amt[2]; z++) {
			ans += dp[amt[0]][amt[1]][amt[2]][z];
		}
		return int(ans);
	}
};

void my_tests() {
	// FILL IN TESTS
	ClassRankings().countWays({50, 50, 50}, {150, 450, 750}, {1000, 1000, 1000});
}

}  // namespace ecnerwala

using ecnerwala::ClassRankings;

// vim:ft=cpp
