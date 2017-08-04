#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e6) + 200;
int N, M;

typedef pair<int, int> pii;
pii W[MAXN];

int solve() {
	int res = 0;
	int mi = W[M].first, ma = W[M].second;
	for (int i = M-1; i >= 0; i--) {
		if (W[i].second <= mi || ma <= W[i].first) {
			// ignore it
		} else {
			res++;
			mi = min(W[i].first, mi);
			ma = max(W[i].second, ma);
		}
	}
	return res;
}

int main() {
	scanf("%d %d", &N, &M);
	M--;
	for (int i = 0; i < N; i++) {
		scanf("%d %d", &W[i].first, &W[i].second);
	}

	int res = solve();
	reverse(W, W + N);
	M = N - 1 - M;
	res = min(res, solve());
	printf("%d\n", res);

	return 0;
}
