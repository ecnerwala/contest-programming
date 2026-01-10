#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		using pt_t = std::array<int, 2>;
		std::vector<pt_t> P; P.reserve(N);
		for (int i = 0; i < N; i++) {
			int x, y, z; cin >> x >> y >> z;
			int a = 2 * x - (y + z);
			int b = 2 * y - (z + x);
			int c = 2 * z - (x + y);
			assert(int64_t(a) + int64_t(b) + int64_t(c) == 0);
			if (a == 0 && b == 0) continue;
			int g = std::gcd(std::abs(a), std::abs(b));
			a /= g, b /= g;
			P.push_back({a, b});
		}

		std::sort(P.begin(), P.end());
		std::vector<std::pair<pt_t, int>> dirs; dirs.reserve(N);
		for (int i = 0, j = 0; i < int(P.size()); i = j) {
			while (j < N && P[j] == P[i]) j++;
			dirs.push_back({P[i], j-i});
		}

		// returns -1, 0, +1, or -2 for exactly reflex
		auto get_dir = [&](pt_t a, pt_t b) -> int {
			int64_t r = int64_t(a[0]) * int64_t(b[1]) - int64_t(a[1]) * int64_t(b[0]);
			if (r) return (r > 0) - (r < 0);
			return int64_t(a[0]) * int64_t(b[0]) + int64_t(a[1]) * int64_t(b[1]) > 0 ? 0 : -2;
		};
		pt_t base_dir = {1, 0};
		auto mid = std::partition(dirs.begin(), dirs.end(), [&](auto a) -> bool {
			return get_dir(base_dir, a.first) < 0;
		});
		std::sort(dirs.begin(), mid, [&](auto a, auto b) -> bool { int d = get_dir(a.first, b.first); assert(d != -2); return d == 1; });
		std::sort(mid, dirs.end(), [&](auto a, auto b) -> bool { int d = get_dir(a.first, b.first); assert(d != -2); return d == 1; });

		// Count ways for 3 distinct directions
		int64_t tot = 0;
		{
			int64_t w1 = 0;
			int64_t w2 = 0;
			for (auto [_, cnt] : dirs) {
				tot += w2 * cnt;
				w2 += w1 * cnt;
				w1 += cnt;
			}
		}

		if (!dirs.empty()) {
			int64_t c1 = 0;
			int64_t c2 = 0;
			for (int i = 0, j = 0; i < N; i++) {
				while (c1 == 0 || get_dir(dirs[j].first, dirs[i].first) < 0) {
					j++;
					if (j == N) j = 0;
				}
				c1 -= dirs[i].second;
				c2 -= c1 * dirs[i].second;
				tot -= c2;
			}
		}

		cout << tot << '\n';
	}

	return 0;
}
