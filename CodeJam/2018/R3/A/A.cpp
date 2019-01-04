#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e4;
int N;
int R[MAXN];
int C[MAXN];

int go() {
	assert(N);
	int mir = R[0], mar = R[0];
	int mic = C[0], mac = C[0];
	for (int i = 1; i < N; i++) {
		mir = min(mir, R[i]);
		mar = max(mar, R[i]);
		mic = min(mic, C[i]);
		mac = max(mac, C[i]);
	}
	return max((mar - mir + 1) / 2, (mac - mic + 1) / 2);
}

int main() {
	int T; scanf("%d", &T);

	for (int case_num = 1; case_num <= T; case_num ++) {

		scanf("%d", &N);
		for (int i = 0; i < N; i++) {
			scanf("%d %d", &R[i], &C[i]);
		}

		int res = go();
		printf("Case #%d: %d\n", case_num, res);
	}

	return 0;
}
