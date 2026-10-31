#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int M; cin >> M;
		std::vector<std::array<int, 3>> V(M, std::array<int, 3>{0, 0, 0});
		for (int i = 0; i < M; i++) {
			string s; cin >> s;
			for (char c : s) {
				if (c == 'w') V[i][0]++;
				else if (c == 'i') V[i][1]++;
				else if (c == 'n') V[i][2]++;
				else assert(false);
			}
		}

		std::array<std::array<std::vector<int>, 3>, 3> edges;
		for (int i = 0; i < M; i++) {
			int excess = -1;
			for (int c = 0; c < 3; c++) {
				if (V[i][c] > 1) {
					assert(excess == -1);
					excess = c;
				}
			}
			for (int c = 0; c < 3; c++) {
				if (V[i][c] == 0) {
					assert(excess != -1);
					edges[excess][c].push_back(i);
				}
			}
		}
		for (int i = 0; i < 3; i++) {
			assert(edges[i][i].empty());
		}

		std::vector<std::tuple<int, int, int, int>> ops; ops.reserve(3*M);
		for (int i = 0; i < 3; i++) {
			for (int j = i+1; j < 3; j++) {
				while (!edges[i][j].empty() && !edges[j][i].empty()) {
					int a = edges[i][j].back(); edges[i][j].pop_back();
					int b = edges[j][i].back(); edges[j][i].pop_back();

					ops.push_back({a, i, b, j});
				}
			}
		}

		{
			int i = 0;
			for (int j = i+1; j < 3; j++) {
				if (i == j) continue;
				int k = 0 + 1 + 2 - i - j;
				while (!edges[i][j].empty()) {
					assert(!edges[j][k].empty() && !edges[k][i].empty());
					int a = edges[i][j].back(); edges[i][j].pop_back();
					int b = edges[j][k].back(); edges[j][k].pop_back();
					int c = edges[k][i].back(); edges[k][i].pop_back();

					ops.push_back({a, i, b, j});
					ops.push_back({b, i, c, k});
				}
			}
		}

		cout << ops.size() << '\n';
		for (auto [a, b, c, d] : ops) {
			cout << a+1 << ' ' << "win"[b] << ' ' << c+1 << ' ' << "win"[d] << '\n';
		}
	}

	return 0;
}
