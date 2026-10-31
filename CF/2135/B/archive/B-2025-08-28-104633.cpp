#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		if (N == -1) exit(0);
		// just walk 2 right, 2 up, 4 left
		std::vector<std::array<int, 2>> anchors(N);
		for (auto& [x, y] : anchors) cin >> x >> y;
		const int64_t J = int(1e9);
		auto query = [&](char d) -> int64_t {
			cout << '?' << ' ' << d << ' ' << J << '\n' << flush;
			int64_t v; cin >> v;
			if (v == -1) exit(0);
			return v;
		};
		query('U');
		query('U');
		query('R');
		int64_t d_ur = query('R');
		query('L');
		query('L');
		query('L');
		int64_t d_ul = query('L');
		int64_t max_ur = -2 * J;
		int64_t max_ul = -2 * J;
		for (auto [x, y] : anchors) {
			max_ur = std::max(max_ur, int64_t(x) + int64_t(y));
			max_ul = std::max(max_ul, int64_t(-x) + int64_t(y));
		}
		int64_t ur_pos = max_ur + d_ur - 4 * J;
		int64_t ul_pos = max_ul + d_ul - 4 * J;
		int64_t y = ur_pos + ul_pos;
		int64_t x = ur_pos - ul_pos;
		assert(x % 2 == 0);
		assert(y % 2 == 0);
		x /= 2;
		y /= 2;
		cout << '!' << ' ' << x << ' ' << y << '\n' << flush;
	}

	return 0;
}
