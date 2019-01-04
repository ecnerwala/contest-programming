#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
int mod_inv(int a, int m) {
	a %= m;
	return a == 1 ? 1 : int(m - ll(mod_inv(m % a, a)) * ll(m) / a);
}

template <int MOD> struct mod_int {
	static_assert(MOD > 1, "MOD should be at least 2");
	int v;
	mod_int(int v_ = 0) : v(v_) { v %= MOD; if (v < 0) v += MOD; assert(0 <= v && v < MOD); }
	mod_int<MOD>& operator += (const mod_int<MOD> &o) {
		int a = v + o.v;
		v = (a >= MOD) ? a-MOD : a;
		return *this;
	}
	mod_int<MOD>& operator -= (const mod_int<MOD> &o) {
		int a = v - o.v;
		v = (a < 0) ? a+MOD : a;
		return *this;
	}
	mod_int<MOD>& operator *= (const mod_int<MOD> &o) {
		int a = int(ll(v) * ll(o.v) % MOD);
		v = a;
		return *this;
	}
	mod_int<MOD>& operator /= (const mod_int<MOD> &o) {
		return operator *= (o.inv());
	}
	mod_int<MOD> operator - () const {
		return mod_int<MOD>{v>0 ? MOD-v : 0};
	}
	mod_int<MOD> inv() const {
		return mod_int<MOD>{mod_inv(v, MOD)};
	}
};

template <int MOD> mod_int<MOD> operator + (const mod_int<MOD> &a, const mod_int<MOD> &b) {
	mod_int<MOD> res = a;
	res += b;
	return res;
}
template <int MOD> mod_int<MOD> operator - (const mod_int<MOD> &a, const mod_int<MOD> &b) {
	mod_int<MOD> res = a;
	res -= b;
	return res;
}
template <int MOD> mod_int<MOD> operator * (const mod_int<MOD> &a, const mod_int<MOD> &b) {
	mod_int<MOD> res = a;
	res *= b;
	return res;
}
template <int MOD> mod_int<MOD> operator / (const mod_int<MOD> &a, const mod_int<MOD> &b) {
	mod_int<MOD> res = a;
	res /= b;
	return res;
}
template <int MOD> ostream& operator << (ostream& o, const mod_int<MOD> &a) {
	return o << a.v;
}

using mint = mod_int<998244353>;

const int MAXN = 1.2e6;
int N;
mint ival[MAXN];
mint fac[MAXN];
mint ifac[MAXN];

void precomp() {
	for (int i = 1; i <= N; i++) {
		ival[i] = mint(i).inv();
	}
	fac[0] = ifac[0] = 1;
	for (int i = 1; i <= N; i++) {
		fac[i] = fac[i-1] * mint(i);
		ifac[i] = ifac[i-1] * ival[i];
	}
}

mint binom(int n, int r) {
	if (!(0 <= r && r <= n)) return 0;
	return fac[n] * ifac[r] * ifac[n-r];
}

mint pow(mint a, int b) {
	mint r = 1;
	while (b) {
		if (b & 1) r *= a;
		a *= a;
		b /= 2;
	}
	return r;
}

int main() {
	cin >> N;

	precomp();

	mint res;
	// case 1: i > 0 and j > 0
	// (-1)^i * (n choose i) * (3^(n-i) - 1)^n * 3
	for (int i = 1; i <= N; i++) {
		mint cur = pow(pow(mint(3), N-i) - mint(1), N) * binom(N, i) * mint(3);
		if (i & 1) cur = -cur;
		res += cur;
	}

	// we have to subtract off j = 0, i.e. sum_i=1^n (-1)^i (n choose i) 3^(n-i)^n * 3 = (3^n - 1)^n * 3 - (3^n)^n * 3
	res -= (pow(pow(mint(3), N) - mint(1), N) - pow(pow(mint(3), N), N)) * mint(3);
	// now add in the j=0 terms
	// sum_i=1^n (-1)^i (n choose i) 3^n^(n-i) * 3^i
	// = (3^n - 3)^n - 3^n^n
	res += pow(pow(mint(3), N) - mint(3), N) * mint(2) - pow(pow(mint(3), N), N);

	cout << pow(pow(mint(3), N), N) - res << '\n';

	return 0;
}
