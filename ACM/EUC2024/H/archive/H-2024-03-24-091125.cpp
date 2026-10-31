#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 2>> cells(N);
		for (auto& [x, y] : cells) cin >> x >> y;
		//cerr << "Start case " << N << '\n';
		auto solve = [&]() -> bool {
			std::sort(cells.begin(), cells.end());
			auto cell_it = cells.begin();

			std::vector<std::pair<int, int>> col;
			col.push_back({0, 1});
			std::vector<std::pair<int, int>> ncol;
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
							if (col_it != col.end() && col_it->first <= cell_y) {
								y = col_it->first;
							} else {
								y = cell_y;
							}
						} else if (col_it != col.end()) {
							y = col_it->first;
						} else {
							break;
						}
					}
					if (cell_it != cells.end() && cell_it->at(0) == x && cell_it->at(1) == y) {
						vy ++;
						++cell_it;
					}
					if (col_it != col.end() && col_it->first == y) {
						vy += col_it->second;
						++col_it;
					}
					//cerr << x << ' ' << y << ' ' << vy << '\n';

					if (vy >= 4) return false;
					if (vy > 0) {
						vy--;
					}
					if (vy > 0) {
						ncol.push_back({y, vy});
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
