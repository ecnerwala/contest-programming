#include<bits/stdc++.h>
using namespace std;

const int MAXN = 310;
int N;
bool G[MAXN][MAXN];

void flipY() {
	for (int i = 0; i < N; i++) {
		reverse(G[i], G[i] + N);
	}
}
void flipX() {
	for (int i1 = 0, i2 = N-1; i1 < i2; i1++, i2--) {
		for (int j = 0; j < N; j++) {
			swap(G[i1][j], G[i2][j]);
		}
	}
}

int pref[MAXN][MAXN];
int solveDiag() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			pref[i][j] = (i-1>=0 && j+1<N ? pref[i-1][j+1] : 0) + G[i][j];
		}
	}
	int ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int l = 1; l <= min(i,j); l++) {
				if (!(G[i-l][j] && G[i][j-l])) continue;
				int xhi = i+l-1, yhi = j+1;
				if (xhi >= N) {
					xhi = N-1;
					yhi = i+j+l - xhi;
				}
				if (yhi < N) ans += pref[xhi][yhi];
				if (j+l < N) ans -= pref[i][j+l];
			}
		}
	}
	return ans;
}

int solveRect() {
	int ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int l = 1; l < N; l++) {
				int cnt = (i-l >= 0 && G[i-l][j]) + (i+l < N && G[i+l][j]) + (j-l >= 0 && G[i][j-l]) + (j+l < N && G[i][j+l]);
				if (cnt == 3) {
					ans++;
				} else if (cnt == 4) {
					ans += 4;
				}
			}
		}
	}
	return ans;
}

int main() {
	if (fopen("triangles.in", "r")) {
		freopen("triangles.in", "r", stdin);
		freopen("triangles.out", "w", stdout);
	}
	cin >> N;
	for (int i = 0; i < N; i++) {
		string S; cin >> S;
		for (int j = 0; j < N; j++) {
			G[i][j] = (S[j] == '*');
		}
	}

	int ans = solveRect();
	for (int fx = 0; fx < 2; fx++, flipX()) {
		for (int fy = 0; fy < 2; fy++, flipY()) {
			ans += solveDiag();
		}
	}
	cout << ans << '\n';

	return 0;
}
