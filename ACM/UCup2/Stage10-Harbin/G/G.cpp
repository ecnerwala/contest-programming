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
		if (N == 1 || M == 1) return true;
		if (M > 2 * K + 1) {
			return false;
		}
		std::vector<std::pair<int, std::array<int, 2>>> empties;
		empties.reserve(M + K);
		for (int y = 0, i = 0; y < M; y++) {
			int x = 0;
			for (; i < K && pts[i].first == y; i++) {
				assert(x <= pts[i].second[0]);
				if (x < pts[i].second[0]) {
					empties.push_back({y, {x, pts[i].second[0]}});
				}
				x = pts[i].second[1];
			}
			assert(x <= N);
			if (x < N) empties.push_back({y, {x, N}});
		}

		std::vector<int> par(int(empties.size()), -1);
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

		for (int i = 0, j = 0; j < int(empties.size()); ) {
			if (empties[j].first <= empties[i].first) {
				j++;
				continue;
			}
			if (empties[j].first - empties[i].first > 1) {
				i++;
				continue;
			}
			assert(empties[j].first - empties[i].first == 1);
			int overlap = -(std::max(empties[i].second[0], empties[j].second[0]) - std::min(empties[i].second[1], empties[j].second[1]));
			if (overlap >= 2) return false;
			else if (overlap == 1) {
				if (!merge(i, j)) return false;
			}
		}

		return true;
	};

	cout << (solve() ? "YES" : "NO") << '\n';

	return 0;
}
