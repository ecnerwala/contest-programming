#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = 998244353;

const int MAXN = 1.2e6;
int N;
int minv[MAXN];

inline int pow(int a, int b, int c=1)  __attribute__((always_inline));
inline int pow(int a, int b, int c) {
	ll r = c;
#define R(__i) do {if (b & (1 << (__i))) r = int(ll(r) * a % MOD);} while(false)
#define A do { a = int(ll(a) * a % MOD); } while(false)
	R(0);
	A;
	R(1);
	A;
	R(2);
	A;
	R(3);
	A;
	R(4);
	A;
	R(5);
	A;
	R(6);
	A;
	R(7);
	A;
	R(8);
	A;
	R(9);
	A;
	R(10);
	A;
	R(11);
	A;
	R(12);
	A;
	R(13);
	A;
	R(14);
	A;
	R(15);
	A;
	R(16);
	A;
	R(17);
	A;
	R(18);
	A;
	R(19);
#undef R
#undef A
	return int(r);
}

int main() {
	scanf("%d", &N);

	ll res = 0;

	// case 1: i > 0 and j > 0
	// (-1)^i * (n choose i) * (3^(n-i) - 1)^n * 3
	ll binom = (N & 1) ? -1 : 1;
	ll p3 = 1;
	minv[1] = 1;
	for (int i = 2; i <= N; i++) {
		minv[i] = int(minv[MOD % i] * (MOD - MOD / i) % MOD);
	}
	for (int ni = 0; ni < N; ni++) {
		int cur = pow(int(p3 - 1), N, int(binom));
		res += cur;

		/*
		p3 *= 3;
		p3 = (p3 >= MOD) ? (p3 - MOD) : p3;
		p3 = (p3 >= MOD) ? (p3 - MOD) : p3;
		*/
		p3 = p3 * 3 % MOD;

		binom *= minv[ni+1];
		binom %= MOD;
		binom *= ni-N;
		binom %= MOD;
	}

	res -= pow(int(p3 - 1), N);
	res *= 3;
	res += 2 * pow(int(p3 - 3), N);
	res = - pow(int(p3), N) - res;

	res %= MOD;
	res = (res < 0) ? res + MOD : res;

	printf("%d\n", int(res));

	return 0;
}
