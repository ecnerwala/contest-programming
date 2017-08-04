#include<bits/stdc++.h>
using namespace std;

const int MAXW = 400, MAXH = 400;
int H, W;
bool G[MAXH][MAXW];
bool G2[MAXH][MAXW];

bool check_row(bool arr[MAXH][MAXW], int i) {
	int cnt[3] = {0, 0, 0};
	for (int j = 0; j < W; j++) {
		cnt[j % 3] ^= arr[i][j];
	}
	return (cnt[0] == cnt[1]) && (cnt[0] == cnt[2]);
}
bool check_col(bool arr[MAXH][MAXW], int j) {
	int cnt[3] = {0, 0, 0};
	for (int i = 0; i < H; i++) {
		cnt[i % 3] ^= arr[i][j];
	}
	return (cnt[0] == cnt[1]) && (cnt[0] == cnt[2]);
}

bool do_step() {
	memcpy(G2, G, sizeof(G));

	int bx = -1, by = -1;
	for (int i = 0; i < H; i++) {
		if (!check_row(G2, i)) bx = i;
	}
	for (int j = 0; j < W; j++) {
		if (!check_col(G2, j)) by = j;
	}

	if ((bx == -1) != (by == -1)) return false;
	if (bx != -1 && by != -1) G2[bx][by] ^= 1;

	for (int i = 0; i < H; i++) {
		if (!check_row(G2, i)) {
			return false;
		}
	}
	for (int j = 0; j < W; j++) {
		if (!check_col(G2, j)) {
			return false;
		}
	}

	for (int i = 0; i < H; i++) {
		int cnt[3] = {0, 0, 0};
		for (int j = 0; j < W; j++) {
			cnt[j % 3] ^= G2[i][j];
			G2[i][j] = cnt[0] ^ cnt[1] ^ cnt[2] ^ cnt[j % 3];
		}
	}

	for (int j = 0; j < W; j++) {
		int cnt[3] = {0, 0, 0};
		for (int i = 0; i < H; i++) {
			cnt[i % 3] ^= G2[i][j];
			G2[i][j] = cnt[0] ^ cnt[1] ^ cnt[2] ^ cnt[i % 3];
		}
	}

	return true;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> W >> H;
	for (int i = 0; i < H; i++) {
		string s; cin >> s;
		for (int j = 0; j < W; j++) {
			G[i][j] = (s[j] == '#');
		}
	}
	while (true) {
		if (!do_step()) {
			break;
		}
		int minx = H;
		int maxx = -1;
		int miny = W;
		int maxy = -1;
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				if (G2[i][j]) {
					minx = min(minx, i);
					maxx = max(maxx, i);
					miny = min(miny, j);
					maxy = max(maxy, j);
				}
			}
		}
		assert((minx <= maxx) == (miny <= maxy));
		if (!(minx <= maxx && miny <= maxy)) {
			break;
		}

		H = maxx - minx + 1;
		W = maxy - miny + 1;
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				G[i][j] = G2[i + minx][j + miny];
			}
		}
	}
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			cout << (G[i][j] ? '#' : '.');
		}
		cout << '\n';
	}
	return 0;
}
