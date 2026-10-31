#include<bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;
typedef __uint128_t L;
struct FastMod {
	ull b, m;
	FastMod(ull b_) : b(b_), m(ull((L(1) << 64) / b_)) {}
	ull reduce(ull a) {
		ull q = (ull)((L(m) * a) >> 64);
		ull r = a - q * b; // can be proven that 0 <= r < 2*b
		return r >= b ? r - b : r;
	}
};

using i64 = int64_t;
using i32 = int32_t;

i64 modinv(i64 a, i64 m) {
	assert(a);
	return a == 1 ? 1 : m - modinv(m % a, a) * m / a;
}

const int MAXN = 8000;
i32 stirling[MAXN][MAXN];
i32 prefStirling[MAXN][MAXN];

int main() {
	if (fopen("exercise.in", "r")) {
		freopen("exercise.in", "r", stdin);
		freopen("exercise.out", "w", stdout);
	}
	int N; cin >> N;
	i64 XMOD; cin >> XMOD;
	const i64 EMOD = XMOD-1;
	FastMod F_EMOD(EMOD);

	vector<i64> fact(N+1);
	fact[0] = 1;
	for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * i % EMOD;

	//vector<vector<i64>> stirling(N+1, vector<i64>(N+1));
	stirling[0][0] = 1;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			stirling[i][j] = i32(F_EMOD.reduce(stirling[i-1][j-1] + i64(i-1) * stirling[i-1][j]));
		}
	}

	const int S = 1 << 13;
	vector<i64> seg(2*S);
	for (int i = 0; i < S; i++) seg[S+i] = i;
	for (int i = S-1; i; i--) seg[i] = seg[2*i] * seg[2*i+1] % EMOD;

	// n! / r!
	auto perm = [&](int n, int r) -> i64 {
		i64 res = 1;
		for (int a = S+r+1, b = S+n+1; a < b; a /= 2, b /= 2) {
			if (a & 1) res = res * seg[a++] % EMOD;
			if (b & 1) res = res * seg[--b] % EMOD;
		}
		return res;
	};

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			stirling[i][j] *= (j & 1 ? 1 : -1);
			if (stirling[i][j] < 0) stirling[i][j] += EMOD;
		}
	}

	//vector<vector<i64>> prefStirling(N+1, vector<i64>(N+1));
	for (int l = 1; l <= N; l++) {
		i64 p = perm(N, l);
		for (int k = 1; k <= l; k++) {
			prefStirling[l][k] = i32(F_EMOD.reduce(stirling[l][k] * p + prefStirling[l-1][k]));
		}
	}

	auto numCycMulSlow = [&](int q) -> i64 {
		i64 res = 0;
		i64 totFac = 1;
		for (int exc = N+1; exc > 0; ) {
			exc--;
			int nexc = exc / q * q;
			totFac = totFac * perm(exc, nexc) % EMOD;
			exc = nexc;
		}
		for (int l = N/q; l > 0; totFac = F_EMOD.reduce(totFac*l*q), l--) {
			i64 curFac = totFac;
			for (int k = l; k > 0; curFac = F_EMOD.reduce(curFac*q), k--) {
				res = F_EMOD.reduce(res + stirling[l][k] * curFac);
			}
		}
		return res;
	};

	auto numCycMulFast = [&](int q) -> i64 {
		assert(__gcd(i64(q), EMOD) == 1);
		i64 invQ = modinv(q % EMOD, EMOD);
		i64 powInvQ = 1;
		i64 res = 0;
		for (int k = 1; k <= N/q; k++) {
			powInvQ *= invQ, powInvQ %= EMOD;
			res = F_EMOD.reduce(res + prefStirling[N/q][k] * powInvQ);
		}
		return res;
	};

	auto numCycMul = [&](int q) -> i64 {
		if (__gcd(i64(q), EMOD) == 1) {
			return numCycMulFast(q);
		} else {
			return numCycMulSlow(q);
		}
	};

	vector<bool> isPrime(N+1, true);
	isPrime[0] = isPrime[1] = false;

	i64 ans = 1;
	for (int p = 2; p <= N; p++) {
		if (!isPrime[p]) continue;
		for (int j = p+p; j <= N; j += p) isPrime[j] = false;
		for (int q = p; q <= N; q *= p) {
			i64 e = numCycMul(q);
			//cerr << q << ' ' << e << '\n';
			i64 v = p;
			while (e) {
				if (e & 1) ans = ans * v % XMOD;
				v = v * v % XMOD;
				e /= 2;
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
