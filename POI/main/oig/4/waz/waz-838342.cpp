#include<bits/stdc++.h>
using namespace std;

const int mx[4] = {-1, 0, 1, 0};
const int my[4] = { 0, 1, 0,-1};

const int MAXN = 1010;
const int MAXM = 1010;
const int MAXR = int(1e6) + 200;
int N, M, R;

int G[MAXN][MAXM]; // last visited time

int main() {
	scanf("%d %d %d", &N, &M, &R);
	int d;
	{
		char dir;
		scanf(" %c", &dir);
		if (dir == 'N') {
			d = 0;
		} else if (dir == 'E') {
			d = 1;
		} else if (dir == 'S') {
			d = 2;
		} else if (dir == 'W') {
			d = 3;
		} else assert(false);
	}
	int cx = -1, cy = -1;
	for (int i = 0; i < N; i++) {
		char s[MAXM];
		scanf(" %s", s);
		for (int j = 0; j < M; j++) {
			if (s[j] == 'W') {
				cx = i, cy = j;
			} else if (s[j] == 'J') {
				G[i][j] = -1;
			} else if (s[j] == '.'){
				G[i][j] = 0;
			} else assert(false);
		}
	}
	assert(cx != -1);

	int l = 1;
	for (int t = 1; t <= R; t ++) {
		char v; scanf(" %c", &v);
		if (v == 'P') {
			d += 1;
			d %= 4;
		} else if (v == 'L') {
			d += 3;
			d %= 4;
		} else if (v == 'N') {
			// do nothing
		} else assert(false);

		cx += mx[d];
		cy += my[d];
		if (!(0 <= cx && cx < N && 0 <= cy && cy < M) || (t - G[cx][cy]) < l) {
			printf("%d\n", t);
			return 0;
		}
		if (G[cx][cy] == -1) {
			l ++;
		}
		G[cx][cy] = t;

	}
	printf("OK\n");
	return 0;
}
