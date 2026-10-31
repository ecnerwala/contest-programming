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

using num = modnum<998244353>;

class RecursiveTournament {

public:

	int count(vector <string> graph, int K) {
		int B = int(graph.size());
		vector<int32_t> adj(B);
		for (int i = 0; i < B; i++) {
			for (int j = 0; j < B; j++) {
				if (i == j || graph[i][j] == 'Y') {
					adj[i] |= (1<<j);
				}
			}
		}
		vector<int32_t> canVis(1<<B);
		for (int i = 0; i < B; i++) {
			canVis[1<<i] = 1<<i;
		}
		for (int m = 1; m < (1<<B); m ++) {
			for (int i = __builtin_ctz(m)+1; i < B; i++) {
				if (!(m & (1<<i))) continue;
				if (adj[i] & canVis[m^(1<<i)]) {
					canVis[m] |= (1<<i);
				}
			}
		}
		vector<int> numCC(B+1);
		for (int m = 1; m < (1<<B); m++) {
			if (adj[__builtin_ctz(m)] & canVis[m]) {
				// this is a good mask
				numCC[__builtin_popcount(m)]++;
			}
		}
		num ans = pow(num(B), K);
		vector<num> p2(K);
		p2[0] = 2;
		for (int i = 1; i < K; i++) {
			p2[i] = pow(p2[i-1], B);
		}
		for (int i = 2; i <= B; i++) {
			num coeff = 0;
			for (int d = 0; d < K; d++) {
				coeff += pow(p2[d] - 1, i) * pow(num(B), K-1-d);
			}
			ans += coeff * numCC[i];
		}
		return int(ans);
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::RecursiveTournament;
