#include<bits/stdc++.h>
#include <numeric>

namespace kactl {
typedef long long ll;
typedef unsigned long long ull;
typedef __uint128_t L;

struct Barrett {
	ull d, m;
	Barrett() {}
	explicit Barrett(ull d_) : d(d_), m(ull((L(1) << 64) / d)) {}

	ull reduce(ull a) {
		ull q = (ull)((L(m) * a) >> 64);
		ull r = a - q * d;
		if (r >= d) r -= d;
		return r;
	}
};
}

template <int& MOD_> struct modnum {
	static constexpr int& MOD = MOD_;
	//static_assert(MOD_ > 0, "MOD must be positive");
	
private:
	using ll = long long;

	static kactl::Barrett fast_mod;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

public:

	static void update_mod() { fast_mod = kactl::Barrett(MOD); }

	modnum() : v(0) {}
	modnum(ll v_) : v(int(fast_mod.reduce(v_))) { if (v < 0) v += MOD; }
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
		v = int(fast_mod.reduce(ll(v) * ll(o.v)));
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

int MOD;

template <> kactl::Barrett modnum<MOD>::fast_mod = {};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	cin >> MOD; // NONPRIME???!!!

	using num = modnum<MOD>;
	num::update_mod();

	vector<num> dp(N+1, num(1));

	vector<num> ndp(N+1);

	vector<num> suff_val(N+1);
	vector<num> pref_val(N+1);
	vector<num> choose; choose.reserve(N+1);
	vector<num> prods; prods.reserve(N+1);

	for (int c = 2; c <= N; c++) {
		if (K % c == 0) {
			copy(dp.begin(), dp.end(), ndp.begin());

			pref_val[0] = 1;
			for (int i = 1; i <= N; i++) {
				if (i % c == 0) pref_val[i] = 1;
				else pref_val[i] = pref_val[i-1] * num(i);
			}
			suff_val[N] = 1;
			for (int i = N; i >= 1; i--) {
				if (i % c == 0) suff_val[i-1] = 1;
				else suff_val[i-1] = suff_val[i] * num(i);
			}

			choose.assign(N/c+1, num(1));
			prods.assign(N/c+1, num(1));

			for (int m = 1; m <= N/c; m++) {
				choose.pop_back();
				for (int v = 1; v < int(choose.size()); v++) choose[v] += choose[v-1];

				prods.pop_back();

				// choose[j] = (j+m choose m)
				for (int i = 0; i + m*c <= N; i++) {
					ndp[i+m*c] += dp[i] * choose[i/c] * suff_val[i] * pref_val[i+m*c] * prods[i/c];
				}

				for (int v = 0; v < int(prods.size()); v++) prods[v] *= suff_val[(v+m) * c];
			}

			swap(dp, ndp);
		}
	}

	cout << dp[N] << '\n';

	return 0;
}

// # of things with cycle structure [c_0 * m_0, c_1 * m1, ...] is exactly N! / prod (c_i^m_i * m_i!)
// why is this so hard without division :(
