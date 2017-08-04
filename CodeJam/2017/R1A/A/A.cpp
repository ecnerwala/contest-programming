#include<bits/stdc++.h>
using namespace std;

const int MAXR = 30, MAXC = 30;
int R, C;
char G[MAXR][MAXC];

void go() {
	for (int i = 0; i < R; i++) {
		for (int j = 1; j < C; j++) {
			if (G[i][j] == '?') {
				G[i][j] = G[i][j-1];
			}
		}
		for (int j = C-2; j >= 0; j--) {
			if (G[i][j] == '?') {
				G[i][j] = G[i][j+1];
			}
		}
	}
	for (int i = 1; i < R; i++) {
		for (int j = 0; j < C; j++) {
			if (G[i][j] == '?') {
				G[i][j] = G[i-1][j];
			}
		}
	}
	for (int i = R - 2; i >= 0; i--) {
		for (int j = 0; j < C; j++) {
			if (G[i][j] == '?') {
				G[i][j] = G[i+1][j];
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> R >> C;
		assert(R < MAXR);
		assert(C < MAXC);
		for (int i = 0; i < R; i++) cin >> G[i];
		go();

		cout << "Case #" << case_num << ":\n";
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				assert(G[i][j] != '?');
				cout << G[i][j];
			}
			cout << '\n';
		}
	}
	return 0;
}
