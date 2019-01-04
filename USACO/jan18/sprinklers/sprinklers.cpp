#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD = ll(1e9) + 7;

const int MAXN = 2e5;
int N;

int H[MAXN];
int Y[MAXN];
ll SY[MAXN];
ll SY2[MAXN];

int main() {
	if(fopen("sprinklers.in", "r")) {
		freopen("sprinklers.in", "r", stdin);
		freopen("sprinklers.out", "w", stdout);
	}
	scanf("%d", &N);
	memset(H, -1, sizeof(H));
	for (int z = 0; z < N; z++) {
		int i, j;
		scanf("%d %d", &i, &j);
		assert(0 <= i && i < N);
		assert(H[i] == -1);
		H[i] = j;
	}
	for (int x = 0; x < N; x++) {
		Y[x] = H[x];
		if (x > 0) Y[x] = min(Y[x], Y[x-1]);
		SY[x+1] = (SY[x] + Y[x]) % MOD;
		SY2[x+1] = (SY2[x] + ll(Y[x]) * Y[x] % MOD) % MOD;
	}
	for (int x = N-1; x > 0; x--) {
		H[x-1] = max(H[x-1], H[x]);
	}
	ll res = 0;
	for (int x = 0; x < N; x++) {
		int mi = -1;
		int ma = x;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			assert(0 <= md && md < x);
			if (Y[md] < H[x]) {
				ma = md;
			} else {
				mi = md;
			}
		}
		ll y = H[x] % MOD;
		ll s0 = (x - ma) % MOD;
		ll s1 = SY[x] - SY[ma]; s1 %= MOD; if (s1 < 0) s1 += MOD;
		ll s2 = SY2[x] - SY2[ma]; s2 %= MOD; if (s2 < 0) s2 += MOD;

		ll v = y * y % MOD * s0 % MOD;
		v += s2; v %= MOD;
		v += y * s0 % MOD; v %= MOD;
		v -= s1 % MOD; v %= MOD; if (v < 0) v += MOD;
		v *= (MOD+1) / 2; v %= MOD;
		v -= y * s1 % MOD; v %= MOD; if (v < 0) v += MOD;

		/*
		cerr << y << ' ' << x << ' ' << ma << '\n';
		cerr << s0 << ' ' << s1 << ' ' << s2 << '\n';
		cerr << v << '\n';
		*/

		res += v; res %= MOD;

	}
	res %= MOD;
	if (res < 0) res += MOD;

	printf("%d\n", int(res % MOD));

	return 0;
}
