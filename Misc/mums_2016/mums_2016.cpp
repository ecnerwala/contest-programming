#include<bits/stdc++.h>
using namespace std;

const int T = 25;
int D[30][8] = {
	{ 1, 1, 1, 3, 2, 1, },
	{ 0, 0, 0, 0, 0, 0, },
	{ 3, 4, 1, 2, 3, 3, },
	{ 5, 1, 2, 1, 3, 4, },
	{ 1, 3, 2, 2, 4, 5, },
	{ 5, 2, 3, 2, 5, 1, },
	{ 1, 5, 0, 0, 0, 0, },
	{ 2, 1, 0, 0, 0, 0, },
	{ 1, 4, 0, 0, 0, 0, },
	{ 4, 4, 0, 0, 0, 0, },
	{ 4, 3, 0, 0, 0, 0, },
	{ 5, 4, 0, 0, 0, 0, },
	{ 4, 5, 0, 0, 0, 0, },
	{ 3, 3, 0, 0, 0, 0, },
	{ 3, 1, 0, 0, 0, 0, },
	{ 2, 4, 0, 0, 0, 0, },
	{ 3, 2, 0, 0, 0, 0, },
	{ 5, 3, 0, 0, 0, 0, },
	{ 2, 5, 0, 0, 0, 0, },
	{ 2, 2, 0, 0, 0, 0, },
	{ 3, 5, 0, 0, 0, 0, },
	{ 4, 1, 0, 0, 0, 0, },
	{ 1, 2, 0, 0, 0, 0, },
	{ 4, 2, 0, 0, 0, 0, },
	{ 5, 5, 0, 0, 0, 0, },
	{0,},
}; // set of dials, 0-indexed

int cnt_same(int a, int b) {
	int res = 0;
	for(int i = 0; i < 6; i++) res += (D[a][i] == D[b][i]);
	return res;
}

void dfs(int t, int d) {
	//cerr << t << ' ' << d << '\n';
	if(t >= T) {
		for(int i = 0; i < T; i++) {
			for(int j = 0; j < 6; j++) {
				cout << D[i][j] << ' ';
			}
			cout << '\n';
		}
		cout << '\n';
	} else if(d == 5) {
		D[t][d] = (t % 5) + 1;
		// let's verify
		bool good = true;
		for(int u = 0; u < t; u++) {
			if(cnt_same(u, t) != 1) {
				good = false;
				break;
			}
		}
		if(good) {
			dfs(t + 1, 0);
		}
	} else if(D[t][d] != 0) {
		if(t > 0 && d == D[t - 1][5] - 1) {
			//if(D[t][d] != D[t-1][d] % 5 + 1) return;
		}
		dfs(t, d + 1);
	} else if(t > 0 && d == D[t - 1][5] - 1) {
		D[t][d] = D[t - 1][d] % 5 + 1;
		dfs(t, d + 1);
		D[t][d] = 0;
	} else {
		assert(D[t][d] == 0);
		for(int i = 1; i <= 5; i++) {
			D[t][d] = i;
			dfs(t, d + 1);
		}
		D[t][d] = 0;
	}
}

int main() {
	dfs(0, 0);
	return 0;
}
