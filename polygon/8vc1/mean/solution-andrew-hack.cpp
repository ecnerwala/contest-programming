#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

const int MAXN = 1e6;

int N;
ll X[MAXN];
ll P[MAXN];

struct frac {
	ll p, q;
	frac(ll num = 0, ll den = 1) : p(num), q(den) { }
};

bool operator< (const frac &a, const frac &b) {
	return a.p * b.q < b.p * a.q;
}

int main() {
	int start_time = clock();

	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		scanf("%lld", &X[i]);
	}
	sort(X, X + N);

	P[0] = 0;
	for(int i = 0; i < N; i++) {
		P[i + 1] = P[i] + X[i];
	}

	int besti = -1, bestj = -1;
	frac res(-1);
	for(int i = 0; i < N; i++) {
		if(i + 1 < N && X[i] == X[i + 1]) continue;
		for(int j = 0; j <= i && N - j > i; j++) {
			frac val ((P[i] - P[i - j]) + (P[N] - P[N - j]) - 2 * j * X[i], 2 * j + 1);
			if(res < val) {
				res = val;
				besti = i;
				bestj = j;
			}
		}
		if(clock() - start_time > CLOCKS_PER_SEC) break;
	}

	printf("%d\n", 2 * bestj + 1);
	for(int v = besti - bestj; v <= besti; v++) {
		printf("%lld ", X[v]);
	}
	for(int v = N - bestj; v < N; v++) {
		printf("%lld ", X[v]);
	}
	printf("\n");

	return 0;
}
