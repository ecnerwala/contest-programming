#include<bits/stdc++.h>
using namespace std;

int main() {
	int N;
	int M;
	scanf("%d %d", &N, &M);
	int last_tall = -1;
	long long res = 0;
	for (int i = 0; i < N; i++) {
		int h; scanf("%d", &h);
		if (h >= M) last_tall = i;
		res += last_tall + 1;
	}
	printf("%lld\n", res);

	return 0;
}
