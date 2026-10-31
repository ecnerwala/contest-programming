#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::string> G(N);
		for (auto& s : G) {
			s.reserve(M);
			cin >> s;
			assert(int(s.size()) == M);
		}
		std::array<int, 5> freq; freq.fill(0);
		for (int i = -1; i <= N; i++) {
			for (int j = -1; j <= M; j++) {
				int v = (i*2+j)%5;
				auto check = [&](int x, int y) -> void {
					if (0 <= x && x < N && 0 <= y && y < M && G[x][y] == '#') freq[v]++;
				};
				if (0 <= i && i < N && 0 <= j && j < M && G[i][j] == '#') {
					check(i, j);
				} else {
					check(i-1, j);
					check(i+1, j);
					check(i, j-1);
					check(i, j+1);
				}
			}
		}
		int best_v = int(std::min_element(freq.begin(), freq.end()) - freq.begin());
		std::vector<std::string> R = G;
		for (int i = -1; i <= N; i++) {
			for (int j = -1; j <= M; j++) {
				int v = (i*2+j)%5;
				if (v != best_v) continue;
				auto check = [&](int x, int y) -> void {
					if (0 <= x && x < N && 0 <= y && y < M && G[x][y] == '#') R[x][y] = 'S';
				};
				if (0 <= i && i < N && 0 <= j && j < M && G[i][j] == '#') {
					check(i, j);
				} else {
					check(i-1, j);
					check(i+1, j);
					check(i, j-1);
					check(i, j+1);
				}
			}
		}
		for (auto& row : R) cout << row << '\n';
	}

	return 0;
}
