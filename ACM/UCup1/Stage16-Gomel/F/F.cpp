#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T;
	cin >> T;
	while (T--) {
		std::array<double, 3> coords;
		std::array<bool, 3> nonzero;
		for (int i = 0; i < 3; i++) {
			int v; cin >> v;
			nonzero[i] = (v > 0);
			coords[i] = v;
		}

		for (int i = 0; i < 3; i++) { cerr << coords[i] << ' '; } cerr << '\n';

		std::vector<std::pair<std::array<int, 3>, double>> ops;
		ops.reserve(3);

		while (true) {
			int to_delete = -1;
			for (int z = 0; z < 3; z++) {
				if (!nonzero[z]) continue;
				if (to_delete == -1 || coords[z] < coords[to_delete]) {
					to_delete = z;
				}
			}
			if (to_delete == -1) break;

			double x_del = coords[to_delete];
			std::array<int, 3> op_coords;
			double op_dist = 0;
			for (int z = 0; z < 3; z++) {
				if (nonzero[z]) {
					op_coords[z] = 255;
					double ov = coords[z];
					if (255 - x_del <= 1e-9) {
						coords[z] = 0;
						nonzero[z] = false;
					} else {
						coords[z] = coords[z] - (255 - coords[z]) / (255 - x_del) * x_del;
					}
					double d = coords[z] - ov;
					op_dist += d * d;
				} else {
					op_coords[z] = 0;
				}
			}
			nonzero[to_delete] = false;

			ops.push_back({op_coords, std::sqrt(op_dist)});
		}

		std::reverse(ops.begin(), ops.end());
		cout << ops.size() << '\n';
		for (auto [p, d] : ops) {
			d = std::max(d, 0.);
			cout << p[0] << ' ' << p[1] << ' ' << p[2] << ' ' << fixed << setprecision(10) << d << '\n';
		}
	}

	return 0;
}
