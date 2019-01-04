#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2000;
int N, K;
int adj[MAXN][MAXN];

void run_ethan() {
	int cur = 1;
	for (int t = 0; t < 1000; t++) {
	}
}

int main() {
	int _z;
	int T; _z = scanf("%d", &T);
	for (int t = 1; t <= T; t++) {
		_z = scanf("%d %d", &N, &K);
		printf("Case #%d: ", t);
		if (K <= 3 || N <= 2) {
			printf("%d\n", 0);
			printf("%d\n", 1);
			printf("%d %d %d\n", 1, N, 1);
		} else {
			vector<int> vals;
			for (int i = K-1; i > 0; i--) {
				vals.push_back(i);
			}
			if (int(vals.size()) > N-1) {
				vals.resize(size_t(N-1));
			}
			assert(vals.size() >= 2);
			{
				int v = 0;
				for (int i : vals) {
					v += i;
				}
				assert(v-K > 0);
				printf("%d\n", v - K);
			}
			printf("%d\n", int(vals.size()) + 1);
			printf("%d %d %d\n", 1, N, K);
			int cur = 1;
			for (int i = 0; i < int(vals.size()); i++) {
				int v = vals[i];
				int nxt = (i+1 == int(vals.size())) ? N : cur+1;
				printf("%d %d %d\n", cur, nxt, v);
				cur = nxt;
			}
		}
	}

	return 0;
}
