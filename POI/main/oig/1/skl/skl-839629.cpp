#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e6) + 200;
int N;
int A[MAXN];
int K[MAXN];

pair<int, int> E[MAXN];

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d %d", &A[i], &K[i]);
		E[i].first = A[i];
		E[i].second = i;
	}
	sort(E, E + N);
	int cnt = 0;
	for (int i = 0; i < N; ) {
		cnt++;
		int cur = E[i].first;
		int ind = E[i].second;
		i++;
		while (i < N && E[i].first == cur) {
			K[ind] += K[E[i].second];
			K[E[i].second] = 0;
			i++;
		}
	}
	printf("%d\n", cnt);
	for (int i = 0; i < N; i++) {
		if (!K[i]) continue;
		printf("%d %d\n", A[i], K[i]);
	}

	return 0;
}
