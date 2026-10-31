#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 2>> cells(N);
		for (auto& [x, y] : cells) cin >> x >> y;
		cerr << "Start case " << N << '\n';
		auto solve = [&]() -> bool {
			std::sort(cells.begin(), cells.end());
			auto cell_it = cells.begin();

			std::vector<std::pair<int, int>> col;
			col.push_back({0, 1});
			for (int x = 0; true; x++) {
				if (col.empty()) {
					if (cell_it == cells.end()) break;
					x = cell_it->at(0);
				}
				auto col_it = col.begin();
				std::vector<std::pair<int, int>> ncol;
				int vy = 0;
				for (int y = 0; true; y++) {
					if (vy == 0) {
						std::optional<int> cell_y;
						if (cell_it != cells.end() && cell_it->at(0) == x) {
							cell_y = cell_it->at(1);
						}
						std::optional<int> col_y;
						if (col_it != col.end()) {
							col_y = col_it->first;
						}
						if (cell_y && (!col_y || *cell_y < *col_y)) {
							assert(*cell_y >= y);
							y = *cell_y;
						} else if (col_y) {
							assert(*col_y >= y);
							y = *col_y;
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
					cerr << x << ' ' << y << ' ' << vy << '\n';

					if (vy >= 4) return false;
					if (vy - 1 > 0) {
						ncol.push_back({y, vy});
					}
					if (vy > 0) {
						vy --;
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
