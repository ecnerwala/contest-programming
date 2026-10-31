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


class PrettyLiar {

public:

	vector<vector<num>> knapsack(int S, vector<int> v) {
		int N = int (v.size());
		vector<vector<num>> res(N+1, vector<num>(S+1));
		res[0][0] = 1;

		for (int i = 0; i < N; i++) {
			for (int j = i; j >= 0; j--) {
				for (int a = 0; a + v[i] <= S; a++) {
					res[j+1][a+v[i]] += res[j][a] * num(j+1);
				}
			}
		}
		num fact = 1;
		for (int i = N-1; i >= 0; i--) {
			fact *= num(N-i);
			for (int a = 0; a <= S; a++) {
				res[i][a] *= fact;
			}
		}
		return res;
	}

	num convSum(int S, const vector<num>& a, const vector<num>& b) {
		num res = 0;
		num prefB = 0;
		for (int i = S; i >= 0; i--) {
			prefB += b[S - i];
			res += prefB * a[i];
		}
		return res;
	}

	int count(int S, vector <int> kaede, vector <int> kanade) {
		S--;
		vector<vector<num>> v1 = knapsack(S, kaede);
		vector<vector<num>> v2 = knapsack(S, kanade);
		int N = int(kaede.size());
		num res = 0;
		for (int i = 0; i < N; i++) {
			//cerr << convSum(S, v1[i+1], v2[i]) << '\n';
			//cerr << convSum(S, v1[i+1], v2[i+1]) << '\n';
			res += convSum(S, v1[i+1], v2[i]) - convSum(S, v1[i+1], v2[i+1]);
		}
		return int(res);
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::PrettyLiar;

// vim:ft=cpp
