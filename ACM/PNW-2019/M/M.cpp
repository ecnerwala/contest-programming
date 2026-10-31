#include<bits/stdc++.h>
using namespace std;

const int MAXR = 1010;
const int MAXC = 1010;
const int MAXV = MAXR * MAXC * 2;

int R, C;
char G[MAXR][MAXC];

int V;
int par[MAXV];
int getPar(int a) { return par[a] == -1 ? a : (par[a] = getPar(par[a])); }
bool merge(int a, int b) {
	a = getPar(a);
	b = getPar(b);
	if (a == b) {
		return false;
	}
	par[b] = a;
	return true;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> R >> C;
	for (int i = 0; i < R; i++) {
		cin >> G[i];
	}

	memset(par, -1, sizeof(par));

	V = R * (C+1) + C * (R+1) + 1;
	int O = V-1;
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			int l = i * (C+1) + j;
			int r = i * (C+1) + j+1;
			int u = R * (C+1) + j * (R+1) + i;
			int d = R * (C+1) + j * (R+1) + i+1;

			if (i == 0) merge(u, O);
			if (i == R-1) merge(d, O);
			if (j == 0) merge(l, O);
			if (j == C-1) merge(r, O);

			if (G[i][j] == '.') {
				merge(l, r);
				merge(u, d);
				merge(l, u);
			} else if (G[i][j] == '/') {
				merge(l, u);
				merge(r, d);
			} else if (G[i][j] == '\\') {
				merge(l, d);
				merge(r, u);
			} else assert(false);
		}
	}

	int ans = -1;
	for (int i = 0; i < V; i++) {
		ans += par[i] == -1;
	}
	cout << ans << '\n';

	return 0;
}
