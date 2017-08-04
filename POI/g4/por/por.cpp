#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e6) + 200;
int N;
int V[MAXN];

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		int a, b; scanf("%d %d", &a, &b);
		V[i+1] = V[i] + (b-a);
	}
	assert(V[N] == 0);
	sort(V, V + N);

	int res = 1;
	int cnt = 1;
	for (int i = 1; i < N; i++) {
		if (V[i] != V[i-1]) {
			cnt = 0;
		}
		cnt ++;
		res = max(res, cnt);
	}

	printf("%d\n", N - res);

	return 0;
}
