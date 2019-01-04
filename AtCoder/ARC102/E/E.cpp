#include<bits/stdc++.h>
using namespace std;

template <int MOD> struct modnum {
	static_assert(MOD > 0, "MOD must be positive");

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

	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};
using num = modnum<998244353>;

const int MAXV = 5000;
num fact[MAXV];
num ifact[MAXV];
num pow2[MAXV];

num binom(int n, int r) {
	if (0 <= r && r <= n) {
		return fact[n] * ifact[r] * ifact[n-r];
	} else return 0;
}

void precomp() {
	fact[0] = ifact[0] = 1;
	pow2[0] = 1;
	for (int i = 1; i < MAXV; i++) {
		fact[i] = fact[i-1] * i;
		ifact[i] = ifact[i-1] / i;
		assert(fact[i] * ifact[i] == 1);
		pow2[i] = pow2[i-1] * 2;
	}
}

int main() {
	precomp();
	int K, N; cin >> K >> N;
	for (int i = 0; i <= (K - 1) * 2; i++) {
		int numMid = (i % 2 == 0);
		int numPairs = (min(i, (K - 1) * 2 - i) + 1 - numMid) / 2;
		int numOther = K - numMid - 2 * numPairs;
		// pick the count of each
		num res = 0;
		for (int v = 0; v <= numPairs; v++) {
			res += binom(numPairs, v) * binom(N + numOther - 1, v + numOther - 1) * pow2[v];
		}
		if (numMid) {
			for (int v = 0; v <= numPairs; v++) {
				res += binom(numPairs, v) * binom((N - 1) + numOther - 1, v + numOther - 1) * pow2[v];
			}
		}
		cout << res << '\n';
	}

	return 0;
}
