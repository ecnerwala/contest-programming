#include<bits/stdc++.h>
using namespace std;

const int MOD = 1e4;

const int MAXN = int(2e5) + 200;
int N;
int W[MAXN];

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &W[i]);
	}
	sort(W, W + N);
	bool over = false;
	int res = (W[0] == W[N-1]) ? 1 : 2;

	int cnt = 0;
	int prv = -1;
	for (int i = 0; i < N; i++) {
		if (W[i] == prv) {
			cnt ++;
			res *= cnt;
			if (res >= MOD) {
				over = true;
				res %= MOD;
			}
			assert(res < MOD);
		} else {
			cnt = 1;
			prv = W[i];
		}
	}
	if (over) {
		printf("%04d\n", res);
	} else {
		printf("%d\n", res);
	}

	return 0;
}
