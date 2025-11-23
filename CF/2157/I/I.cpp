#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int M = 1; M <= 20; M++) {
		const int N = 120;
		std::vector<std::vector<int>> winning_moves(N);
		std::string res(N, '?');
		for (int i = 0; i < N; i++) {
			if (winning_moves[i].empty()) {
				for (int j = i+1; j < N && j <= i+M; j++) winning_moves[j].push_back(i);
				res[i] = 'X';
			} else if (winning_moves[i].size() == 1) {
				int k = 2 * i - winning_moves[i][0];
				if (k < N) winning_moves[k].push_back(i);
				res[i] = '1';
			} else {
				res[i] = '2';
			}
		}
		cerr << M << '\n';
		cerr << res << '\n';
	}

	return 0;
}
