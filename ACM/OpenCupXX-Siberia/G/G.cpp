#include<bits/stdc++.h>
using namespace std;

const int MAXH = 13;
const int MAXW = 1.1e5;
int H, W;
char G[MAXH][MAXW];

int nextFilled[MAXW];
int nxt[MAXW][20];

int K;
const int MAXQ = 1.1e5;
int Q;
struct {
	int L, R, D;
} queries[MAXQ];
int ans[MAXQ];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> H >> W >> K >> Q;
	for (int i = 0; i < H; i++) {
		cin >> G[i];
	}
	for (int q = 0; q < Q; q++) {
		cin >> queries[q].D >> queries[q].L >> queries[q].R;
		queries[q].L --;
	}

	for (int h = 0; h < H; h++) {
		nextFilled[W] = W;
		for (int l = 0; l < 20; l++) {
			nxt[W][l] = W;
		}
		for (int i = W-1; i >= 0; i--) {
			nextFilled[i] = (G[h][i] == 'X') ? i : nextFilled[i+1];
			nxt[i][0] = nextFilled[min(i + K, W)];
			for (int l = 1; l < 20; l++) {
				nxt[i][l] = nxt[nxt[i][l-1]][l-1];
			}
		}

		for (int q = 0; q < Q; q++) {
			if (h >= queries[q].D) continue;
			int v = nextFilled[queries[q].L];
			int r = queries[q].R;
			if (v >= r) {
				continue;
			}
			for (int l = 19; l >= 0; l--) {
				if (nxt[v][l] < r) {
					v = nxt[v][l];
					ans[q] += (1 << l);
				}
			}
			ans[q] ++;
		}
	}
	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
