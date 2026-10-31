#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, K; cin >> N >> M >> K;
	std::vector<std::pair<int, std::array<int, 2>>> pts;
	for (int i = 0; i < K; i++) {
		int x1, x2, y; cin >> x1 >> x2 >> y; x1--, y--;
		pts.push_back({y, {x1, x2}});
	}
	std::sort(pts.begin(), pts.end());

	auto solve = [&]() -> bool {
		std::vector<int> par(K+1, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[a] >= par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};

		// outside boundary
		for (int i = 0; i < K; i++) {
			if (pts[i].first == 0 || pts[i].first == M-1 || pts[i].second[0] == 0 || pts[i].second[1] == N) {
				merge(i, K);
			}
		}

		for (int i = 0; i+1 < K; i++) {
			assert(pts[i].first <= pts[i+1].first);
			if (pts[i].first < pts[i+1].first) continue;
			assert(pts[i].second[1] <= pts[i+1].second[0]);
			if (pts[i+1].second[0] - pts[i].second[1] == 0) {
				merge(i, i+1);
			}
		}

		for (int i = 0, j = 0; j < K; ) {
			if (pts[j].first <= pts[i].first) {
				j++;
				continue;
			}
			if (pts[j].first - pts[i].first > 1) {
				i++;
				continue;
			}
			assert(pts[j].first - pts[i].first == 1);
			if (std::max(pts[i].second[0], pts[j].second[0]) <= std::min(pts[i].second[1], pts[j].second[1])) {
				merge(i, j);
			}
			if (pts[i].second[1] <= pts[j].second[1]) {
				i++;
			} else {
				j++;
			}
		}

		auto get_comp = [&](int y, int x) -> int {
			if (y < 0 || y >= M || x < 0 || x >= N) return get_par(K);

			auto it = std::lower_bound(pts.begin(), pts.end(), std::pair<int, std::array<int, 2>>{y, {x+1, x+1}});
			if (it == pts.begin()) return -1;
			--it;
			if (it->first == y && it->second[0] <= x && x < it->second[1]) {
				return get_par(int(it - pts.begin()));
			} else {
				return -1;
			}
		};
		auto get_comp_2 = [&](int y0, int x0, int y1, int x1) -> int {
			int r0 = get_comp(y0, x0);
			int r1 = get_comp(y1, x1);
			if (r0 == -1) return r1;
			if (r1 == -1) return r0;
			assert(r0 == r1);
			return r0;
		};

		auto test_vert = [&](int y0, int x0) -> bool {
			if (get_comp_2(y0, x0, y0, x0+1) != -1) return false;
			int a = get_comp_2(y0-1, x0, y0-1, x0+1);
			int b = get_comp_2(y0+1, x0, y0+1, x0+1);
			return a == b && a != -1;
		};
		auto test_horiz = [&](int y0, int x0) -> bool {
			if (get_comp_2(y0, x0, y0+1, x0) != -1) return false;
			int a = get_comp_2(y0, x0-1, y0+1, x0-1);
			int b = get_comp_2(y0, x0+1, y0+1, x0+1);
			return a == b && a != -1;
		};

		for (int i = 0; i < K; i++) {
			if (test_vert(pts[i].first, pts[i].second[1])) return true;
			if (test_vert(pts[i].first-1, std::max(0, pts[i].second[0]-1))) return true;
			if (test_vert(pts[i].first+1, std::max(0, pts[i].second[0]-1))) return true;
			if (test_horiz(pts[i].first-1, pts[i].second[0]-1)) return true;
			if (test_horiz(pts[i].first-1, pts[i].second[1])) return true;
			if (test_horiz(pts[i].first, pts[i].second[0]-1)) return true;
			if (test_horiz(pts[i].first, pts[i].second[1])) return true;
		}

		if (N == 1 || M == 1) {
			int64_t num_filled = 0;
			for (int i = 0; i < K; i++) {
				num_filled += pts[i].second[1] - pts[i].second[0];
			}
			int64_t num_empty = int64_t(N) * int64_t(M) - num_filled;
			if (num_empty >= 2) return true;
		}

		return false;
	};

	cout << (solve() ? "YES" : "NO") << '\n';

	return 0;
}
