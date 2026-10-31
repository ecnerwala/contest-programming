#include <bits/stdc++.h>

[[noreturn]] void FAIL() {
	std::cout << 0 << '\n';
	exit(0);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::vector<bool>> grid(N);
	for (int i = 0; i < N; i++) {
		grid[i].resize(M);
		string s; cin >> s;
		for (int j = 0; j < M; j++) {
			grid[i][j] = s[j] == '1';
		}
	}

	auto must_set = [&](int i, int j) {
		if (i < 0 || j < 0 || i >= N || j >= M) FAIL();
		if (grid[i][j] == 1) {
			FAIL();
		}
		grid[i][j] = 1;
	};

	int num_flips = 0;

	for (int s = 0; s <= N+M-2; s++) {
		for (int i = std::max(0, s-(M-1)), j = s-i; i < N && j >= 0; i++, j--) {
			if (grid[i][j] == 0) {
				must_set(i, j);
				must_set(i, j+2);
				must_set(i+2, j+2);
				must_set(i+2, j);
				if (grid[i+1][j] == 1) {
					must_set(i, j+1);
					must_set(i+1, j+2);
					must_set(i+2, j+1);
					continue;
				}
				if (grid[i][j+1] == 1) {
					must_set(i+1, j);
					must_set(i+1, j+2);
					must_set(i+2, j+1);
					continue;
				}
				if (grid[i+1][j+2] == 1) {
					must_set(i, j+1);
					must_set(i+1, j);
					must_set(i+2, j+1);
					continue;
				}
				if (grid[i+2][j+1] == 1) {
					must_set(i, j+1);
					must_set(i+1, j);
					must_set(i+1, j+2);
					continue;
				}

				if (i+1 < N && j-1 >= 0 && grid[i+1][j-1] == 0) {
					must_set(i+1, j-1);
					must_set(i+1, j+1);
					must_set(i+3, j-1);
					must_set(i+3, j+1);

					must_set(i, j+1);
					must_set(i+1, j);
					must_set(i+1, j+2);
					must_set(i+2, j-1);
					must_set(i+2, j+1);
					must_set(i+3, j);

					num_flips++;
					continue;
				}
				
				must_set(i+1, j);
				must_set(i, j+1);
				// 6 things set so far

				if (grid[i+1][j+1] == 0) {
					must_set(i+1, j+1);
					must_set(i+1, j+2);
					must_set(i+1, j+3);
					must_set(i+2, j+3);
					must_set(i+3, j+3);
					must_set(i+3, j+2);
					must_set(i+3, j+1);
					must_set(i+2, j+1);

					num_flips++;
					continue;
				}

				if (i-1 >= 0 && grid[i-1][j+2] == 0) {
					must_set(i-1, j+2);
					must_set(i-1, j+3);
					must_set(i-1, j+4);
					must_set(i, j+4);
					must_set(i+1, j+4);
					must_set(i+1, j+3);
					must_set(i+1, j+2);

					must_set(i+2, j+1);
					continue;
				}

				if (j-1 >= 0 && grid[i+2][j-1] == 0) {
					must_set(i+2, j-1);
					must_set(i+3, j-1);
					must_set(i+4, j-1);
					must_set(i+4, j);
					must_set(i+4, j+1);
					must_set(i+3, j+1);
					must_set(i+2, j+1);

					must_set(i+1, j+2);
					continue;
				}

				if (j+3 < M && grid[i+2][j+3] == 0) {
					must_set(i+2, j+3);
					must_set(i+3, j+3);
					must_set(i+4, j+3);
					must_set(i+4, j+2);
					must_set(i+4, j+1);
					must_set(i+3, j+1);
					must_set(i+2, j+1);

					must_set(i+1, j+2);
					continue;
				}

				if (i+3 < N && grid[i+3][j+2] == 0) {
					must_set(i+3, j+2);
					must_set(i+3, j+3);
					must_set(i+3, j+4);
					must_set(i+2, j+4);
					must_set(i+1, j+4);
					must_set(i+1, j+3);
					must_set(i+1, j+2);

					must_set(i+2, j+1);
					continue;
				}

				FAIL();
			}
		}
	}

	const int MOD = 998244353;
	int ans = 1;
	for (int z = 0; z < num_flips; z++) {
		ans *= 2;
		if (ans >= MOD) ans -= MOD;
	}
	cout << ans << '\n';

	return 0;
}
