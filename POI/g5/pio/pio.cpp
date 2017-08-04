#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll gcd(ll a, ll b) {
	return a ? gcd(b % a, a) : b;
}

const int MAXN = int(2e5) + 200;
int N[2];
int X[2][2];
ll A[2][MAXN];

int main() {
	scanf("%d %d", &N[0], &N[1]);
	scanf("%d %d %d %d", &X[0][0], &X[1][0], &X[0][1], &X[1][1]);
	X[0][0] --, X[1][0] --, X[0][1] --, X[1][1] --; // shift the center to 0,0
	for (int z = 0; z < 2; z++) {
		for (int i = 0; i < N[z]; i++) {
			scanf("%lld", &A[z][i]);
		}
	}

	ll res = 1;
	for (int z = 0; z < 2; z++) {
		ll g = 0;
		for (int i = 0; i < N[z]; i++) {
			g = gcd(g, A[z][i]);
		}
		assert(g);
		ll s = X[z][0] - 1, t = X[z][1]; // (s,t]
		if (s < 0) {
			ll v = (-s) / g + 1;
			s += v * g;
			t += v * g;
		}
		res *= t / g - s / g;
	}
	printf("%lld\n", res);


	return 0;
}
