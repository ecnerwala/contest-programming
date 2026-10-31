#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int R, C; cin >> R >> C;
		std::vector<std::string> G(R);
		for (auto& r : G) { cin >> r; assert(int(r.size()) == C); }
		std::vector<std::string> origG = G;
		for (int x = 0; x < R; x++) {
			for (int y = 0; y < C; y++) {
				if (G[x][y] == '.') G[x][y] = '^';
				assert(G[x][y] == '^' || G[x][y] == '#');
			}
		}

		int N = R*C;
		std::vector<int> deg(N, 0);
		for (int x = 0; x < R; x++) {
			for (int y = 0; y < C; y++) {
				if (x+1 < R && G[x][y] != '#' and G[x+1][y] != '#') {
					deg[x*C+y]++;
					deg[(x+1)*C+y]++;
				}
				if (y+1 < C && G[x][y] != '#' and G[x][y+1] != '#') {
					deg[x*C+y]++;
					deg[x*C+(y+1)]++;
				}
			}
		}

		std::vector<int> q; q.reserve(N);
		for (int v = 0; v < N; v++) {
			if (G[v/C][v%C] == '^' && deg[v] <= 1) {
				G[v/C][v%C] = '.';
				q.push_back(v);
			}
		}
		for (int z = 0; z < int(q.size()); z++) {
			int v = q[z];
			for (auto [mx, my] : {std::pair<int, int>{1, 0}, {-1, 0}, {0, -1}, {0, 1}}) {
				int x = v/C + mx, y = v%C + my;
				if (0 <= x && x < R && 0 <= y && y < C && G[x][y] == '^') {
					deg[x*C+y]--;
					if (deg[x*C+y] == 1) {
						G[x][y] = '.';
						q.push_back(x*C+y);
					}
				}
			}
		}

		cout << "Case #" << case_num << ": ";
		bool possible = true;
		for (int x = 0; x < R; x++) {
			for (int y = 0; y < C; y++) {
				assert((origG[x][y] == '#') == (G[x][y] == '#'));
				if (G[x][y] == '.' && origG[x][y] == '^') {
					possible = false;
				}
			}
		}
		if (possible) {
			cout << "Possible" << '\n';
			for (auto& r : G) cout << r << '\n';
		} else {
			cout << "Impossible" << '\n';
		}
	}

	return 0;
}
