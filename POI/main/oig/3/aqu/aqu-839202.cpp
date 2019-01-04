#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 2020;
int N;
ll S[MAXN][MAXN];

int R;

int main() {
	scanf("%d %d", &N, &R);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			int v; scanf("%d", &v);

			int x = i + j + 1;
			int y = i - j + N;
			assert(1 <= x && x < 2 * N);
			assert(1 <= y && y < 2 * N);
			S[x][y] = v;
		}
	}
	for (int i = 1; i < 2 * N; i++) {
		for (int j = 1; j < 2 * N; j++) {
			S[i][j] += S[i-1][j] + S[i][j-1] - S[i-1][j-1];
		}
	}

	for (int r = 0; r < R; r++) {
		int i, j, l; scanf("%d %d %d", &i, &j, &l);
		i--, j--;

		int x = i + j + 1;
		int y = i - j + N;
		assert(1 <= x && x < 2 * N);
		assert(1 <= y && y < 2 * N);
		int x0 = max(0, x - l - 1), x1 = min(x + l, 2 * N - 1);
		int y0 = max(0, y - l - 1), y1 = min(y + l, 2 * N - 1);

		ll v = S[x1][y1] - S[x1][y0] - S[x0][y1] + S[x0][y0];
		printf("%lld\n", v);
	}

	return 0;
}
