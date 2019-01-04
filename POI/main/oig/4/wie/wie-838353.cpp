#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;
const int MAXN = int(5e5) + 200;
const int MAXW = int(1e6) + 200;
const int MAXK = int(1e6) + 200;
int N;

int W[MAXN];
int K;
int X[MAXK];

int B;
int vals[MAXN];

ll best[MAXN];

int main() {
	scanf("%d", &N);
	memset(X, 0, sizeof(X));
	for (int i = 0; i < N; i++) {
		scanf("%d", &W[i]);
		for (int j = 0; j < W[i]; j++) {
			scanf("%d", &X[K++]);
		}
	}
	B = 0;
	for (int i = 0; i < N; i++) {
		vals[B++] = W[i];
	}
	assert(B == N);
	sort(vals, vals + B);
	B = int(unique(vals, vals + B) - vals);
	for (int i = 0; i < B; i++) {
		best[i] = -INF;
	}
	K = 0;
	for (int i = 0; i < N; i++) {
		int ind = 0;
		ll cv = 0;
		for (int j = 0; j < W[i]; j++) {
			cv += X[K++];
			assert(ind < B);
			if (j+1 == vals[ind]) {
				best[ind] = max(best[ind], cv);
				ind++;
				cv = 0;
			}
		}
		assert(cv == 0);
	}
	ll res = 0;
	for (int i = B-1; i >= 0; i--) {
		res = max(res, 0ll) + best[i];
	}
	printf("%lld\n", res);

	return 0;
}
