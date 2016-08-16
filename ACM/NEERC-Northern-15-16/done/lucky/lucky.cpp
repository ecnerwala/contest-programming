#include<bits/stdc++.h>
using namespace std;

int N, M;
int grid[200][200];

int res = 0;

void go() {
	for(int i = 0; i < N; i++) {
		int m = 0;
		for(int j = 0; j < M; j++) {
			if(grid[i][j] > m) {
				m = grid[i][j];
				res ++;
			}
		}
	}
}

int main() {
	if(fopen("lucky.in", "r")) {
		freopen("lucky.in", "r", stdin);
		freopen("lucky.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0);

	cin >> N >> M;
	for(int i = 0; i < N; i++) for(int j = 0; j < M; j++) cin >> grid[i][j];
	
	go();
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M - 1 - j; j++) {
			swap(grid[i][j], grid[i][M - 1 - j]);
		}
	}
	go();

	for(int i = 0; i < max(N, M); i++) {
		for(int j = i + 1; j < max(N, M); j++) {
			swap(grid[i][j], grid[j][i]);
		}
	}
	swap(N, M);

	go();
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M - 1 - j; j++) {
			swap(grid[i][j], grid[i][M - 1 - j]);
		}
	}
	go();

	cout << res << '\n';
}
