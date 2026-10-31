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

const int MAXN = 8000;
i32 stirling[MAXN * MAXN / 8];

int main() {
	if (fopen("exercise.in", "r")) {
		freopen("exercise.in", "r", stdin);
		freopen("exercise.out", "w", stdout);
	}
	int N; cin >> N;
	i32 XMOD; cin >> XMOD;
	const i32 EMOD = XMOD-1;
	FastMod F_EMOD(EMOD);

	stirling[0] = EMOD-1;
	int x = 1;
	for (int i = 1; i <= N/2; i++) {
		x++; // skip j = 0
		for (int j = 1; j <= i; j++) {
			stirling[x] = i32(F_EMOD.reduce((EMOD-stirling[x-i-1]) + i64(i-1) * stirling[x-i]));
			x++;
		}
	}

	auto numCycMul = [&](int q) -> i64 {
		i64 res = 0;
		i64 totFac = 1;
		for (int i = 1; i <= N; i++) {
			if (i % q == 0) continue;
			totFac = F_EMOD.reduce(totFac * i);
		}
		int a = (N/q+1)*(N/q+2) / 2;
		for (int l = N/q; l > 0; totFac = F_EMOD.reduce(totFac*l*q), l--) {
			i64 curFac = totFac;
			for (int k = l; k > 0; curFac = F_EMOD.reduce(curFac*q), k--) {
				res = F_EMOD.reduce(res + stirling[--a] * curFac);
			}
			--a; // skip k = 0
		}
		return res;
	};

	vector<bool> isPrime(N+1, true);
	isPrime[0] = isPrime[1] = false;

	FastMod F_XMOD(XMOD);

	i64 ans = 1;
	for (int p = 2; p <= N; p++) {
		if (!isPrime[p]) continue;
		for (int j = p+p; j <= N; j += p) isPrime[j] = false;
		for (int q = p; q <= N; q *= p) {
			i64 e = numCycMul(q);
			//cerr << q << ' ' << e << '\n';
			i64 v = p;
			while (e) {
				if (e & 1) ans = F_XMOD.reduce(ans * v);
				v = F_XMOD.reduce(v * v);
				e /= 2;
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
