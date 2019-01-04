#include<bits/stdc++.h>
using namespace std;

const int MAXX = 1e9;
	
const int MAXN = 2e5;
int N;
int X[MAXN];

const int MAXD = 2e5;
int D;
int P[MAXD][2];

int main() {
	scanf("%d %d", &N, &D);
	for (int i = 0; i < N; i++) {
		scanf("%d", &X[i]);
		assert(-MAXX < X[i] && X[i] < MAXX);
	}
	if (D == 0) {
		printf("0\n");
		return 0;
	}
	for (int i = 0; i < D; i++) {
		scanf("%d %d", &P[i][0], &P[i][1]);
		P[i][0] --;
		P[i][1] --;
		assert(P[i][0] != P[i][1]);
	}

	int minF = MAXX, maxF = -MAXX;
	int minB = MAXX, maxB = -MAXX;
	for (int i = 0; i < D; i++) {
		int s = X[P[i][0]], t = X[P[i][1]];
		assert(s != t);
		if (s < t) {
			minF = min(minF, s);
			maxF = max(maxF, t);
			assert(minF <= maxF);
		} else {
			minB = min(minB, t);
			maxB = max(maxB, s);
			assert(minB <= maxB);
		}
	}
	if (minF > maxF) {
		assert(minB <= maxB);
		printf("%d\n", maxB - minB);
		return 0;
	}
	if (minB > maxB) {
		assert(minF <= maxF);
		printf("%d\n", maxF - minF);
		return 0;
	}
	assert(minF <= maxF);
	assert(minB <= maxB);
	int res = min(2 * max(maxF, maxB) - minF - minB, maxF + maxB - 2 * min(minF, minB));
	printf("%d\n", res);
	return 0;
}
