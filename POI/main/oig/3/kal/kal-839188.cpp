#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

const int MAXN = int(1e6) + 200;
int N[2];
int A[2][MAXN];

int main() {
	scanf("%d %d", &N[0], &N[1]);
	for (int z = 0; z < 2; z++) {
		A[z][0] = 0;
		for (int i = 0; i < N[z]; i++) {
			int v; scanf("%d", &v);
			A[z][i+1] = A[z][i] + v;
		}
	}

	int Q; scanf("%d", &Q);
	for (int i = 0; i < Q; i++) {
		int d, m; char c;
		scanf("%d %d %c", &d, &m, &c); d--, m--;
		bool z = (c == 'B');

		int cur_day = A[z][m] + d;
		assert(cur_day < A[z][m+1]);
		assert(cur_day >= 0);

		m = int(upper_bound(A[!z], A[!z] + N[!z], cur_day) - A[!z] - 1);
		assert(m >= 0);
		d = cur_day - A[!z][m];

		printf("%d %d\n", d+1, m+1);
	}


	return 0;
}
