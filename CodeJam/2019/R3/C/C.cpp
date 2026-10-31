#include<bits/stdc++.h>
using namespace std;

const int MAXR = 110;
const int MAXC = 110;
int R, C;

const int MAXV = MAXR * MAXC;
int V;
int par[MAXV];
void reset() {
	for (int i = 0; i < V; i++) par[i] = i;
}
int getpar(int a) {
	return (par[a] == a) ? a : (par[a] = getpar(par[a]));
}
bool merge(int a, int b) {
	a = getpar(a), b = getpar(b);
	if (a == b) return false;
	par[a] = b;
	return true;
}

int vert(int i, int j) {
	return i * C + j;
}

char G[MAXR][MAXC];

char ans[MAXR][MAXC];

void go() {
	cin >> R >> C;
	for (int i = 0; i < R; i++) {
		cin >> G[i];
	}
	
	V = R * C;
	reset();

	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			if (i + 1 < R && G[i][j] == G[i+1][j]) {
				merge(vert(i, j), vert(i+1, j));
			}
			if (j + 1 < C && G[i][j] == G[i][j+1]) {
				merge(vert(i, j), vert(i, j+1));
			}
		}
	}

	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			if (i + 1 < R && j + 1 < C) {
				ans[i][j] = '.';
				if (ans[i][j] == '.' && G[i][j] == G[i+1][j+1]) {
					if (merge(vert(i, j), vert(i+1, j+1))) {
						ans[i][j] = '\\';
					}
				}
				if (ans[i][j] == '.' && G[i+1][j] == G[i][j+1]) {
					if (merge(vert(i+1, j), vert(i, j+1))) {
						ans[i][j] = '/';
					}
				}
			}
		}
	}

	unordered_set<int> s;
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			s.insert(getpar(vert(i, j)));
		}
	}
	if (s.size() != 2) {
		cout << "IMPOSSIBLE\n";
	} else {
		cout << "POSSIBLE\n";
		for (int i = 0; i+1 < R; i++) {
			for (int j = 0; j+1 < C; j++) {
				cout << ans[i][j];
			}
			cout << '\n';
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		cout << "Case #" << case_num << ": ";
		go();
	}

	return 0;
}
