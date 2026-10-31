#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e3;
char ans[MAXN][MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			ans[i][j] = '.';
		}
	}
	if (N <= 2) {
		cout << -1 << '\n';
		exit(0);
	}
	if (N == 3) {
		cout << ".aa" << '\n';
		cout << "a.." << '\n';
		cout << "a.." << '\n';
		exit(0);
	}

	int v;
	if (N % 4 == 0) {
		v = 0;
	} else if (N % 4 == 1) {
		ans[0][0] = 'a';
		ans[0][1] = 'a';
		ans[0][2] = 'b';
		ans[0][3] = 'b';
		ans[0][4] = 'a';
		ans[1][4] = 'a';
		ans[2][4] = 'b';
		ans[3][4] = 'b';
		ans[4][4] = 'a';
		ans[4][3] = 'a';
		ans[4][2] = 'b';
		ans[4][1] = 'b';
		ans[4][0] = 'a';
		ans[3][0] = 'a';
		ans[2][0] = 'b';
		ans[1][0] = 'b';

		ans[1][2] = 'c';
		ans[1][3] = 'c';

		ans[2][1] = 'c';
		ans[3][1] = 'c';

		v = 5;
	} else if (N % 4 == 2) {
		ans[0][1] = 'a';
		ans[0][2] = 'a';

		ans[1][0] = 'a';
		ans[2][0] = 'a';

		ans[0][3] = 'b';
		ans[0][4] = 'b';
		ans[0][5] = 'c';
		ans[1][5] = 'c';
		ans[2][5] = 'b';
		ans[2][4] = 'b';
		ans[2][3] = 'c';
		ans[1][3] = 'c';

		ans[3][0] = 'b';
		ans[3][1] = 'b';
		ans[3][2] = 'c';
		ans[4][2] = 'c';
		ans[5][2] = 'b';
		ans[5][1] = 'b';
		ans[5][0] = 'c';
		ans[4][0] = 'c';

		ans[3][4] = 'a';
		ans[3][5] = 'a';

		ans[4][3] = 'a';
		ans[5][3] = 'a';
		v = 6;
	} else if (N % 4 == 3) {
		ans[0][0] = 'a';
		ans[0][1] = 'a';
		ans[0][2] = 'b';
		ans[0][3] = 'b';
		ans[1][0] = 'b';
		ans[1][3] = 'z';
		ans[1][4] = 'z';
		ans[2][0] = 'b';
		ans[2][1] = 'a';
		ans[3][1] = 'a';
		ans[3][4] = 'a';
		ans[3][5] = 'a';
		ans[4][2] = 'a';
		ans[4][5] = 'b';
		ans[5][2] = 'a';
		ans[5][5] = 'b';

		ans[0][6] = 'x';
		ans[1][6] = 'x';
		ans[2][6] = 'y';
		ans[3][6] = 'y';
		ans[4][6] = 'z';
		ans[5][6] = 'z';

		ans[6][0] = 'x';
		ans[6][1] = 'x';
		ans[6][2] = 'y';
		ans[6][3] = 'y';
		ans[6][4] = 'z';
		ans[6][5] = 'z';

		v = 7;
	} else assert(false);
	for (; v < N; v += 4) {
		ans[v+0][v+0] = 'a';
		ans[v+0][v+1] = 'a';
		ans[v+1][v+0] = 'b';
		ans[v+1][v+1] = 'b';

		ans[v+2][v+2] = 'a';
		ans[v+2][v+3] = 'a';
		ans[v+3][v+2] = 'b';
		ans[v+3][v+3] = 'b';

		ans[v+0][v+2] = 'c';
		ans[v+0][v+3] = 'd';
		ans[v+1][v+2] = 'c';
		ans[v+1][v+3] = 'd';

		ans[v+2][v+0] = 'c';
		ans[v+2][v+1] = 'd';
		ans[v+3][v+0] = 'c';
		ans[v+3][v+1] = 'd';
	}
	assert(v == N);

	for (int i = 0; i < N; i++) {
		ans[i][N] = '\0';
		cout << ans[i] << '\n';
	}

	return 0;
}
