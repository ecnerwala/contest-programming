#include<bits/stdc++.h>
using namespace std;

const int MAXK = 8000;
int N, K;
pair<int, int> S[MAXK];
int res[MAXK];

const int D = 10;
const int B = D * 10;
int seen[B];

int dsum(int a) {
	int r = 0;
	for (; a; a /= 10) {
		r += a % 10;
	}
	return r;
}

int main() {
	scanf("%d %d", &N, &K);
	for (int i = 0; i < K; i++) {
		scanf("%d", &S[i].first);
		S[i].second = i;
	}
	sort(S, S + K);

	int cnt = 0;
	int cur = 0;

	memset(seen, 0, sizeof(seen));
	for (int i = 1; i <= N; i++) {
		if (!seen[i%B]) {
			cnt ++;
			while (cur < K && S[cur].first == cnt) {
				res[S[cur].second] = i;
				cur ++;
			}
		} else {
			seen[i%B] = false;
		}

		int nxt = i + dsum(i);
		assert(dsum(i) < B);
		seen[nxt%B] = true;
	}

	printf("%d\n", cnt);
	for (int i = 0; i < K; i++) {
		printf("%d%c", res[i], " \n"[i + 1 == K]);
	}

	return 0;
}
