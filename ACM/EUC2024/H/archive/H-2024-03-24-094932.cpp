#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 2>> cells(N);
		for (auto& [x, y] : cells) cin >> x >> y;
		/*
		int N = 200000;
		std::vector<std::array<int, 2>> cells(N);
		for (int i = 0; i < N/2; i++) cells[i] = {0, i};
		for (int i = N/2; i < N; i++) cells[i] = {1+i-(N/2), 0};
		*/
		//cerr << "Start case " << N << '\n';
		auto solve = [&]() -> bool {
			std::sort(cells.begin(), cells.end());
			auto cell_it = cells.begin();

			// half open ranges
			std::vector<std::pair<std::pair<int, int>, int>> col;
			col.push_back({{0, 1}, 1});
			std::vector<std::pair<std::pair<int, int>, int>> ncol;
			for (int x = 0; true; x++) {
				if (col.empty()) {
					if (cell_it == cells.end()) break;
					x = cell_it->at(0);

				}
				auto col_it = col.begin();
				ncol.clear();
				int vy = 0;
				for (int y = 0; true; y++) {
					if (vy == 0) {
						if (cell_it != cells.end() && cell_it->at(0) == x) {
							int cell_y = cell_it->at(1);
							if (col_it != col.end() && col_it->first.first <= cell_y) {
								y = col_it->first.first;
							} else {
								y = cell_y;
							}
						} else if (col_it != col.end()) {
							y = col_it->first.first;
						} else {
							break;
						}
					}
					if (cell_it != cells.end() && cell_it->at(0) == x && cell_it->at(1) == y) {
						vy ++;
						++cell_it;
					}
					if (col_it != col.end() && col_it->first.first == y) {
						if (col_it->second == 1) {
							// a whole run of 1s, pretty much does nothing
							int yhi = col_it->first.second;
							if (cell_it != cells.end() && cell_it->at(0) == x && cell_it->at(1) < yhi) {
								yhi = cell_it->at(1);
								col_it->first.first = yhi;
							} else {
								++col_it;
							}
							//cerr << x << ' ' << y << '-' << yhi << ' ' << vy+1 << '\n';
							if (vy > 0) {
								if (vy == 1) {
									if (vy == 1 && !ncol.empty() && ncol.back().second == 1 && ncol.back().first.second == y) {
										ncol.back().first.second = yhi;
									} else {
										ncol.push_back({{y, yhi}, vy});
									}
								} else {
									assert(vy >= 2);
									if (yhi - y >= 3) {
										return false;
									}
									for (int a = y; a < yhi; a++) {
										ncol.push_back({{a, a+1}, vy});
									}
								}
							}
							y = yhi - 1;
							continue;
						} else {
							assert(col_it->first.second == y+1);
							vy += col_it->second;
							++col_it;
						}
					}
					//cerr << x << ' ' << y << ' ' << vy << '\n';

					assert(vy > 0);
					if (vy >= 4) return false;
					if (vy > 0) {
						vy--;
					}
					if (vy > 0) {
						if (vy == 1 && !ncol.empty() && ncol.back().second == 1 && ncol.back().first.second == y) {
							ncol.back().first.second = y+1;
						} else {
							ncol.push_back({{y, y+1}, vy});
						}
					}
				}
				std::swap(col, ncol);
			}
			return true;
		};
		bool ans = solve();
		cout << (ans ? "YES" : "NO") << '\n';
	}

	return 0;
}
