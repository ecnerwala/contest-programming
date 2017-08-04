#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = int(1e6) + 200;
int N;
ll K;
bool has[MAXN];

int main() {
	scanf("%d %lld", &N, &K);
	for (int i = 0; i < N; i++) {
		ll v; scanf("%lld", &v);
		if (v % K) continue;
		v /= K;
		if (v > N) { v = N + 1; }
		has[v] = true;
	}
	int res = 1;
	while (has[res]) res++;
	printf("%lld\n", K * ll(res));

	return 0;
}
