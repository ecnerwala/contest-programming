#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, M; cin >> N >> M;
	vector<vector<int>> grid(N, vector<int>(M));
	for (int j = 0; j < M; j++) {
		for (int i = 0; i < N; i++) {
			cin >> grid[i][j];
		}
	}

	vector<vector<bool>> isGood(N, vector<bool>(M, true));
	for (int i = 0; i < N; i++) {
		int best = int(1e9)+100;
		for (int j = 0; j < M; j++) {
			best = min(best, grid[i][j]);
		}

		int numBest = 0;
		for (int j = 0; j < M; j++) {
			if (grid[i][j] == best) numBest++;
		}
		for (int j = 0; j < M; j++) {
			if (numBest >= 2 || grid[i][j] != best) isGood[i][j] = false;
		}
	}

	for (int j = 0; j < M; j++) {
		int best = 0;
		for (int i = 0; i < N; i++) {
			best = max(best, grid[i][j]);
		}
		int numBest = 0;
		for (int i = 0; i < N; i++) {
			if (grid[i][j] == best) numBest++;
		}
		for (int i = 0; i < N; i++) {
			if (numBest >= 2 || grid[i][j] != best) isGood[i][j] = false;
		}
	}

	for (int j = 0; j < M; j++) {
		for (int i = 0; i < N; i++) {
			if (isGood[i][j]) {
				cout << j+1 << ' ' << i+1 << '\n';
				exit(0);
			}
		}
	}
	cout << 0 << ' ' << 0 << '\n';

	return 0;
}
