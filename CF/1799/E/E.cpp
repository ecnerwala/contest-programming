#include <bits/stdc++.h>

template <typename T> void setmin(T &x, T y) { if (y < x) x = y; }
template <typename T> void setmax(T &x, T y) { if (y > x) x = y; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::string> G(N);
		for (int i = 0; i < N; i++) {
			cin >> G[i];
		}

		std::vector<bool> vis(N*M);
		std::vector<std::vector<std::array<int, 2>>> cities;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (G[i][j] == '#' && !vis[i*M+j]) {
					std::vector<std::array<int, 2>> q; q.reserve(N*M);
					vis[i*M+j] = true;
					q.push_back({i, j});
					for (int z = 0; z < int(q.size()); z++) {
						auto check = [&](int nx, int ny) {
							if (0 <= nx && nx < N && 0 <= ny && ny < M && G[nx][ny] == '#' && !vis[nx*M+ny]) {
								vis[nx*M+ny] = true;
								q.push_back({nx, ny});
							}
						};
						auto [x, y] = q[z];
						check(x-1, y);
						check(x+1, y);
						check(x, y-1);
						check(x, y+1);
					}
					cities.push_back(q);
				}
			}
		}
		assert(cities.size() == 2);
		struct box_t {
			int min_x, max_x, min_y, max_y;
		};
		std::vector<box_t> boxes(2);
		for (int z = 0; z < 2; z++) {
			int min_y = M;
			int max_y = -1;
			int min_x = N;
			int max_x = -1;

			for (auto [x, y] : cities[z]) {
				setmin(min_x, x);
				setmax(max_x, x);
				setmin(min_y, y);
				setmax(max_y, y);
			}
			boxes[z] = {min_x, max_x, min_y, max_y};
		}

		if ((boxes[0].max_x < boxes[1].min_x || boxes[0].min_x > boxes[1].max_x)
			&& (boxes[0].max_y < boxes[1].min_y || boxes[0].min_y > boxes[1].max_y)) {

			int x0 = (boxes[0].max_x < boxes[1].min_x) ? boxes[0].max_x : boxes[0].min_x;
			int x1 = (boxes[0].max_x < boxes[1].min_x) ? boxes[1].min_x : boxes[1].max_x;

			int y0 = (boxes[0].max_y < boxes[1].min_y) ? boxes[0].max_y : boxes[0].min_y;
			int y1 = (boxes[0].max_y < boxes[1].min_y) ? boxes[1].min_y : boxes[1].max_y;

			G[x1][y0] = '#';
		}

		std::vector<int> row_min_y(N, M);
		std::vector<int> row_max_y(N, -1);
		for (int x = 0; x < N; x++) {
			for (int y = 0; y < M; y++) {
				if (G[x][y] == '#') {
					setmin(row_min_y[x], y);
					setmax(row_max_y[x], y);
				}
			}
		}
		{
			auto mid = int(std::max_element(row_max_y.begin(), row_max_y.end()) - row_max_y.begin());
			for (int i = 1; i < mid; i++) {
				setmax(row_max_y[i], row_max_y[i-1]);
			}
			for (int i = N-2; i > mid; i--) {
				setmax(row_max_y[i], row_max_y[i+1]);
			}
		}
		{
			auto mid = int(std::min_element(row_min_y.begin(), row_min_y.end()) - row_min_y.begin());
			for (int i = 1; i < mid; i++) {
				setmin(row_min_y[i], row_min_y[i-1]);
			}
			for (int i = N-2; i > mid; i--) {
				setmin(row_min_y[i], row_min_y[i+1]);
			}
		}
		for (int x = 0; x < N; x++) {
			assert((row_min_y[x] == M && row_max_y[x] == -1) || row_min_y[x] <= row_max_y[x]);
			for (int y = 0; y < M; y++) {
				if (row_min_y[x] <= y && y <= row_max_y[x]) {
					G[x][y] = '#';
				} else {
					assert(G[x][y] == '.');
				}
			}
			cout << G[x] << '\n';
		}
	}

	return 0;
}
