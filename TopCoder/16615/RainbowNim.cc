#ifndef ECNERWALA_MACHINE
#define NDEBUG
#include <numeric>
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

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

using num = modnum<998244353>;
const num INV2 = inv(num(2));

class RainbowNim {

	vector<num> base_2_conv(vector<num> a, vector<num> b) {
		int L = 0, S = 1;
		while (S < int(a.size()) || S < int(b.size())) {
			L++, S *= 2;
		}
		a.resize(S, num(0));
		b.resize(S, num(0));
		// just run the FFT, who cares
		for (int i = 0; i < L; i++) {
			for (int m = 0; m < S; m++) {
				if (m & (1 << i)) continue;
				int n = m + (1 << i);
				tie(a[m], a[n]) = make_pair(a[m] + a[n], a[m] - a[n]);
				tie(b[m], b[n]) = make_pair(b[m] + b[n], b[m] - b[n]);
			}
		}
		for (int m = 0; m < S; m++) {
			a[m] *= b[m];
		}
		for (int i = 0; i < L; i++) {
			for (int m = 0; m < S; m++) {
				if (m & (1 << i)) continue;
				int n = m + (1 << i);
				tie(a[m], a[n]) = make_pair((a[m] + a[n]) * INV2, (a[m] - a[n]) * INV2);
			}
		}
		return a;
	}

	vector<num> solve_color(int M, vector<int> piles) {
		int P = *max_element(piles.begin(), piles.end());
		int S = M+1;
		while (S <= P) S *= 2;
		vector<num> dp(S);
		dp[0] = num(1);
		vector<num> pdp(S);
		for (int p : piles) {
			assert(p < S);
			copy(dp.begin(), dp.end(), pdp.begin());
			for (int m = 0; m < S; m++) {
				int nm = (p + m) % (M+1) + ((p / (M+1)) ^ (m / (M+1))) * (M+1);
				dp[nm] += pdp[m];
			}
		}
		return dp;
	}

public:

	int countSubsets(vector <int> pile, vector <int> color, int m) {
		int C = *max_element(color.begin(), color.end()) + 1;
		vector<vector<int>> piles(C+1);
		for (int i = 0; i < int(pile.size()); i++) {
			piles[color[i]].push_back(pile[i]);
		}
		vector<num> res({num(1)});
		for (const vector<int>& c : piles) {
			if (c.empty()) continue;
			res = base_2_conv(std::move(res), solve_color(m, c));
		}
		cerr << res.size() << '\n';
		assert(std::accumulate(res.begin(), res.end(), num(0)) == pow(num(2), int(pile.size())));
		return int(std::accumulate(res.begin()+1, res.end(), num(0)));
	}

	// This is a sum-game between different colors. If all piles are small (<= m), it's sum mod m+1
	// If there's 1 big pile, then its value is just the big pile
	// I believe it's base-m+1, base-2, base-2, base-2
};

void my_tests() {
	// FILL IN TESTS
	cerr << "{";
	for (int i = 0; i < 250; i++) {
		if (i) cerr << ',';
		cerr << 250;
	}
	cerr << "}" << '\n';
	cerr << "{";
	for (int i = 0; i < 250; i++) {
		if (i) cerr << ',';
		cerr << i+1;
	}
	cerr << "}" << '\n';
	cerr << 128 << '\n';
}

}  // namespace ecnerwala

using ecnerwala::RainbowNim;

// vim:ft=cpp
