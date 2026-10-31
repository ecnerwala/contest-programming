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

class MaxOfMin {

public:

	int count(vector <int> F) {
		int N = int(F.size());
		F.insert(F.begin(), N+1);
		for (int i = 0; i < N; i++) {
			if (!(F[i] >= F[i+1])) return 0;
		}
		if (F[N] != 1) return 0;

		vector<int> run_lengths(N+1);
		run_lengths[0] = N;
		for (int i = 1; i <= N; i++) {
			run_lengths[i] = run_lengths[i-1];
			while (i >= F[run_lengths[i]]) run_lengths[i]--;
		}
		assert(run_lengths[N] == 0);

		using num = modnum<998244353>;
		map<vector<int>, num> dp;
		dp[vector<int>({N})] = num(1);
		for (int i = 1; i <= N; i++) {
			map<vector<int>, num> ndp;
			for (const auto& it : dp) {
				const auto& s = it.first;
				num v = it.second;
				// pick a split point
				for (int z = 0; z < int(s.size()); ) {
					int nz = z+1;
					while (nz < int(s.size()) && s[nz] == s[z]) nz++;
					num cur_v = v * num(nz - z);
					for (int x = 0; x < s[z]; x++) {
						vector<int> ns = s;
						ns[z] = x;
						ns.push_back(s[z]-1-x);
						sort(ns.begin(), ns.end(), std::greater<int>());
						if (ns[0] == run_lengths[i]) {
							ndp[ns] += cur_v;
						}
					}
					z = nz;
				}
			}
			dp = std::move(ndp);
		}

		num ans = 0;
		for (const auto& it : dp) ans += it.second;
		return int(ans);
	}
};

void my_tests() {
	int N = 50;
	vector<int> F(N);
	iota(F.begin(), F.end(), 1);
	reverse(F.begin(), F.end());
	fill(F.begin()+25, F.end(), 1);
	cerr << '{'; for (auto v : F) { cerr << v << ','; } cerr << '}' << '\n';
	cerr << MaxOfMin().count(F) << '\n';
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::MaxOfMin;

// vim:ft=cpp
