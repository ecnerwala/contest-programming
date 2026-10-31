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

using ll = long long;

class BlackAndWhiteBalls {

public:

	num solve(vector<ll> balls, vector<ll> needs) {
		int N = int(balls.size());

		balls.insert(balls.begin(), 1ll);
		balls.push_back(1ll);
		assert(int(balls.size()) == N+2);

		swap(needs[0], needs[1]);

		vector<vector<num>> dp(N+2, vector<num>(2));
		dp[0][1] = 1;
		dp[1][0] = 1;

		for (int i = 0; i <= N; i++) {
			// need to cover it exactly
			for (bool z : {!!(i&1), !(i&1)}) {
				num curCnt = dp[i][z];
				ll m = needs[z];

				// ways to choose 0 <= a <= b < l such that a == b (mod m)
				auto choose2Same = [&m](ll l) -> num {
					assert(l >= 0);
					return num(l/m) * num(l/m + 1) / 2 * num(m) + num(l/m+1) * num(l%m);
				};

				if (z == (i & 1)) {
					// start at any point
					// first thing you could do is not make it out
					// pick any two with the same mod
					dp[i][!z] += curCnt * choose2Same(balls[i]);
				}

				ll poss = (z == (i&1)) ? balls[i] : 1;
				ll pref = (z == (i&1)) ? 1 : 0;

				for (int j = i+1; j <= N+1; j++) {
					if ((j & 1) == z) {
						num ways = 0;

						if (pref == 0) {
							assert(poss == 1);
							ways = (balls[j] - 1) / m;
						} else {
							// trying to pick -poss < a <= 0 <= pref <= b < pref + balls[j] with a == b (mod m)
							ways += choose2Same(pref + balls[j] + poss - 1);
							ways -= choose2Same(pref + balls[j] - 1);
							ways -= choose2Same(pref + poss - 1);
							ways += choose2Same(pref - 1);
						}

						dp[j][!z] += curCnt * ways;

						pref += balls[j];
					} else {
						assert(pref);
						dp[j][!z] += curCnt * ((pref + poss - 1) / m - (pref - 1) / m);
					}
				}
			}
		}
		return dp[N+1][0] + dp[N+1][1];
	}

	int getNumber(vector <int> balls, int white, int black) {
		return int(solve(vector<ll>(balls.begin(), balls.end()), vector<ll>({ll(white), ll(black)})));
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::BlackAndWhiteBalls;

// vim:ft=cpp
