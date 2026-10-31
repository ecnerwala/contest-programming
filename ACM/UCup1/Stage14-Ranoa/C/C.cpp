#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::vector<int>> grid(N);
	for (int i = 0; i < N; i++) {
		grid[i].resize(i+1);
		string s; cin >> s;
		for (int j = 0; j <= i; j++) grid[i][j] = s[j] == '1';
	}

	auto can_solve = [&]() -> bool {
		for (int flip_long = 0; flip_long < 2; flip_long++) {
			for (int i = 0; i < N; i++) grid[i][i] ^= 1;
			for (int flip_long2 = 0; flip_long2 < 2; flip_long2++) {
				for (int i = 1; i < N; i++) grid[i][i-1] ^= 1;

				for (int flip_top = 0; flip_top < 2; flip_top++) {
					grid[0][0] ^= 1;

					for (int i = 1; i < N; i++) {
						if (grid[i-1][i-1]) {
							// flip col i-1
							for (int j = i-1; j < N; j++) grid[j][i-1] ^= 1;
						}
						if (grid[i][i-1]) {
							for (int j = 0; j <= i; j++) grid[i][j] ^= 1;
						}
					}
					if (grid[N-1][N-1]) {
						grid[N-1][N-1] ^= 1;
					}

					for (int i = 2; i < N; i++) {
						if (grid[i][0]) {
							for (int j = i; j < N; j++) grid[j][j-i] ^= 1;
						}
					}

					bool all_good = true;
					for (int i = 0; i < N; i++) {
						for (int j = 0; j <= i; j++) {
							all_good = all_good && (grid[i][j] == 0);
						}
					}
					if (all_good) return true;
				}
			}
		}
		return false;
	};

	bool res = can_solve();
	cout << (res ? "Yes" : "No") << '\n';
	return 0;
}
