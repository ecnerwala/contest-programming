#include<bits/stdc++.h>
using namespace std;

const int MAXR = 110;
const int MAXC = 110;
int R, C;
int A[MAXR], B[MAXC];
int G[MAXR][MAXC];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> R >> C;
	for (int i = 0; i < R; i++) {
		cin >> A[i];
	}
	for (int j = 0; j < C; j++) {
		cin >> B[j];
	}
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			G[i][j] = min(A[i], B[j]);
		}
	}
	bool possible = true;
	for (int i = 0; i < R; i++) {
		bool has = false;
		for (int j = 0; j < C; j++) {
			if (G[i][j] == A[i]) has = true;
		}
		if (!has) {
			possible = false;
		}
	}
	for (int j = 0; j < C; j++) {
		bool has = false;
		for (int i = 0; i < R; i++) {
			if (G[i][j] == B[j]) has = true;
		}
		if (!has) {
			possible = false;
		}
	}
	cout << (possible ? "possible" : "impossible") << '\n';

	return 0;
}
