#include<bits/stdc++.h>
using namespace std;

const int MAXR = 200;
const int MAXC = 200;
int R, C;
char G[MAXR][MAXC];

int sum[MAXR + 1][MAXC + 1];

int H, V;
const int MAXH = MAXR;
int horiz[MAXH];
const int MAXV = MAXR;
int vert[MAXV];

bool go() {
	memset(sum, 0, sizeof(sum));
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			sum[i+1][j+1] = sum[i+1][j] + sum[i][j+1] - sum[i][j] + (G[i][j] == '@');
		}
	}
	H ++;
	V ++;
	int T = sum[R][C];
	if (T % (H * V) != 0) {
		return false;
	}
	if (T == 0) return true;

	int TH = T / H;
	memset(horiz, -1, sizeof(horiz));
	for (int i = 0; i <= R; i++) {
		if (sum[i][C] % TH == 0) {
			horiz[sum[i][C] / TH] = i;
		}
	}
	for (int i = 0; i <= H; i++) {
		if (horiz[i] == -1) return false;
	}

	int TV = T / V;
	memset(vert, -1, sizeof(vert));
	for (int j = 0; j <= C; j++) {
		if (sum[R][j] % TV == 0) {
			vert[sum[R][j] / TV] = j;
		}
	}
	for (int j = 0; j <= V; j++) {
		if (vert[j] == -1) return false;
	}

	int TB = T / (H * V);
	for (int i = 0; i <= H; i++) {
		for (int j = 0; j <= V; j++) {
			if (sum[horiz[i]][vert[j]] != i * j * TB) {
				return false;
			}
		}
	}

	return true;
}

int main() {
	int T; scanf("%d", &T);

	for(int case_num = 1; case_num <= T; case_num ++) {
		scanf("%d %d %d %d", &R, &C, &H, &V);
		for (int i = 0; i < R; i++) {
			scanf(" %s", G[i]);
		}
		bool res = go();
		printf("Case #%d: %s\n", case_num, (res ? "POSSIBLE" : "IMPOSSIBLE"));
	}

	return 0;
}
