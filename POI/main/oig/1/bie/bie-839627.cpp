#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(3e4) + 200;
int N;
int V[MAXN];

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &V[i]);
	}
	sort(V, V + N);
	int res = 2;
	for (int l = 0, r = 2; r < N; l++) {
		while (r < N && V[l] + V[l+1] > V[r]) r++;
		res = max(res, r-l);
	}
	printf("%d\n", res);

	return 0;
}
