#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = int(2e5) + 200;
ll N;
ll A, B;
ll X[2][MAXN];

ll K;

ll count_smaller(ll v) {
	ll res = 0;
	for (ll a = N - 1, b = 0; a >= 0; a--) {
		while (b < N && X[0][a] * X[1][b] <= v) {
			b++;
		}
		res += b;
	}
	return res;
}

int main() {
	scanf("%lld %lld %lld %lld", &A, &B, &N, &K);
	for (int z = 0; z < 2; z++) {
		for (int i = 0; i < N; i++) {
			scanf("%lld", &X[z][i]);
		}
	}
	X[0][N] = A;
	X[1][N] = B;
	N++;
	for (int z = 0; z < 2; z++) {
		for (int i = int(N)-1; i > 0; i--) {
			X[z][i] -= X[z][i-1];
		}
	}

	sort(X[0], X[0] + N);
	sort(X[1], X[1] + N);

	ll mi = -1;
	ll ma = X[0][N-1] * X[1][N-1] + 1;
	K = N * N + 1 - K;
	while (ma - mi > 1) {
		ll md = (mi + ma) / 2;
		if (count_smaller(md) >= K) {
			ma = md;
		} else {
			mi = md;
		}
	}
	printf("%lld\n", ma);

	return 0;
}
