#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1000;
int N;
const int MAXC = 50;
int C;
int cnt[3][MAXC];
int D[MAXC][MAXC];

int main() {
	cin >> N >> C;
	for (int i = 0; i < C; i++) {
		for (int j = 0; j < C; j++) {
			cin >> D[i][j];
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			int v; cin >> v; v--;
			cnt[(i+j)%3][v] ++;
		}
	}

	int res = int(1.2e9);
	for (int a = 0; a < C; a++) {
		for (int b = 0; b < C; b++) {
			if (b == a) continue;
			for (int c = 0; c < C; c++) {
				if (c == b || c == a) continue;
				int v = 0;
				for (int x = 0; x < C; x++) {
					v += cnt[0][x] * D[x][a];
					v += cnt[1][x] * D[x][b];
					v += cnt[2][x] * D[x][c];
				}
				res = min(res, v);
			}
		}
	}
	cout << res << '\n';

	return 0;
}
