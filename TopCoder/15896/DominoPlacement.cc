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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

using num = modnum<int(1e9)+7>;

class DominoPlacement {

public:

	int countWays(vector <string> T) {
		int N = int(T.size());
		int M = int(T[0].size());
		if (N < M) {
			// transpose
			vector<string> nt(M, string(N, '\0'));
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < M; j++) {
					nt[j][i] = T[i][j];
				}
			}
			T = nt;
			swap(N, M);
		}
		assert(M <= N);
		assert(M <= 17);
		// first is normal, second means can either
		vector<pair<num, num>> dp(1<<M);
		vector<pair<num, num>> ndp(1<<M);
		dp[0].first++;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				for (int m = 0; m < (1<<M); m++) ndp[m] = {0,0};
				if (T[i][j] == '#') {
					for (int m = 0; m < (1<<M); m++) {
						ndp[m&(~(1<<j))].first += dp[m].first + dp[m].second;
					}
				} else {
					for (int m = 0; m < (1<<M); m++) {
						// neither top nor left
						ndp[m|(1<<j)].second += dp[m].first + dp[m].second;

						// only left
						if (m & (1<<j)) {
							assert((dp[m].first + dp[m].second == 0) || (i && T[i-1][j] != '#'));
							ndp[m | (1<<j)].first += dp[m].first + dp[m].second;
						}
						// only top
						if (j && T[i][j-1] != '#') {
							if (m & (1<<(j-1))) {
								ndp[m&(~(3<<(j-1)))].first += dp[m].second;
							} else {
								ndp[m&(~(3<<(j-1)))].first += dp[m].first + dp[m].second;
							}
						}
					}
				}
				swap(dp, ndp);
			}
		}

		num ans = 0;
		for (int m = 0; m < (1<<M); m++) {
			ans += dp[m].first;
			ans += dp[m].second;
		}
		return int(ans);
	}
};

void my_tests() {
	// FILL IN TESTS
	
	cerr << DominoPlacement().countWays(vector<string>(17, string(18, '.'))) << '\n';
	cerr << DominoPlacement().countWays(vector<string>(18, string(17, '.'))) << '\n';
	mt19937 mt(48);
	vector<string> T(17, string(18, '.'));
	for (auto& s : T) {
		for (auto& c : s) {
			c = (mt() % 3 ? '.' : '#');
		}
	}
	cerr << DominoPlacement().countWays(T) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::DominoPlacement;

// vim:ft=cpp
