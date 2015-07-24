#include<bits/stdc++.h>
using namespace std;

int main() {
	int T; scanf("%d", &T);
	for(int t = 1; t <= T; t++) {
		int N;
		scanf("%d", &N);
		vector<int> M(N);
		for(int i = 0; i < N; i++) {
			scanf("%d", &M[i]);
		}
		int y = 0;
		int z = 0;
		int r = 0;
		for(int i = 1; i < N; i++) {
			int d = M[i - 1] - M[i]; // decrease
			if(d >= 0) {
				y += d;
				r = max(r, d);
			}
		}
		for(int i = 1; i < N; i++) {
			z += min(r, M[i - 1]);
		}
		printf("Case #%d: %d %d\n", t, y, z);
	}
	return 0;
}
