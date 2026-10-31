#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int NT, NS, R; cin >> NT >> NS >> R;
	std::vector<std::array<int, 2>> trees(NT);
	for (auto& [x, y] : trees) cin >> x >> y;
	std::vector<std::array<int, 2>> readings(NS);
	for (auto& [x, y] : readings) cin >> x >> y;

	std::sort(trees.begin(), trees.end());

	auto is_good = [&](int dx, int dy) -> bool {
		int j = 0;
		for (int i = 0; i < NT; i++) {
			int rx = trees[i][0] - dx;
			int ry = trees[i][1] - dy;
			if (rx == 0 && ry == 0) return false;
			if (std::abs(rx) + std::abs(ry) > R) {
				continue;
			} else {
				if (j == NS || readings[j] != std::array<int, 2>{rx, ry}) {
					return false;
				}
				j++;
			}
		}
		return j == NS;
	};

	int num_sol = 0;
	std::array<int, 2> last_sol{-1, -1};

	for (int rot = 0; rot < 4; rot++) {
		std::sort(readings.begin(), readings.end());

		// pick the match for readings[0]
		for (int a0 = 0; a0 < NT; a0++) {
			int dx = trees[a0][0] - readings[0][0];
			int dy = trees[a0][1] - readings[0][1];
			if (is_good(dx, dy)) {
				num_sol++;
				last_sol = {dx, dy};
			}
		}

		for (auto& [x, y] : readings) {
			std::swap(x, y);
			x = -x;
		}
	}
	if (num_sol == 0) {
		cout << "Impossible" << '\n';
	} else if (num_sol == 1) {
		cout << last_sol[0] << ' ' << last_sol[1] << '\n';
	} else {
		cout << "Ambiguous" << '\n';
	}

	return 0;
}
