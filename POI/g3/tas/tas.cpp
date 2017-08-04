#include<bits/stdc++.h>
using namespace std;

int main() {
	int M; scanf("%d", &M);
	while ( M --> 0 ) {
		int N;
		scanf("%d", &N);
		assert(N >= 1);
		int v; scanf("%d", &v);
		int plen = 1;
		int slen = 1;
		for (int i = 1; i < N; i++) {
			int u; scanf("%d", &u);
			if (u == v) {
				if (plen == i) {
					plen++;
				}
				slen++;
			} else {
				slen = 0;
			}
		}
		if (plen == N) {
			printf("BRAK\n");
		} else {
			printf("%d\n", N - min(plen, slen) - 1);
		}
	}

	return 0;
}
