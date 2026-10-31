#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N, K; std::cin >> N >> K;
	std::vector<std::optional<std::array<int, 2>>> locs(N);
	for (int i = 0; i < K; i++) {
		int x, y, f; std::cin >> x >> y >> f; f--;
		locs[f] = {x, y};
	}
	std::vector<std::pair<int, int>> gems(N);
	for (int i = 0; i < N; i++) {
		int x, f; std::cin >> x >> f; f--;
		gems[i] = {x, f};
	}

	std::vector<int> prv(N, -1);
	std::vector<int> nxt(N, -1);
	for (int i = 0; i < N; i++) {
		if (locs[gems[i].second]) prv[i] = i;
		else prv[i] = i == 0 ? -1 : prv[i-1];
	}
	for (int i = N-1; i >= 0; i--) {
		if (locs[gems[i].second]) nxt[i] = i;
		else nxt[i] = i == N-1 ? -1 : nxt[i+1];
	}

	auto sq = [&](int a) -> int64_t {
		return int64_t(a) * int64_t(a);
	};
	auto get_dist = [&](int x, int f) -> int64_t {
		assert(locs[f]);
		auto [gx, gy] = *locs[f];
		return sq(gx - x) + sq(gy);
	};

	for (int i = 0; i < N; i++) {
		if (locs[gems[i].second]) continue;
		assert(prv[i] != -1 || nxt[i] != -1);
		int64_t max_r = 2e18;
		for (auto v : {prv[i], nxt[i]}) {
			if (v == -1) continue;
			max_r = std::min(max_r, get_dist(gems[i].first, gems[v].second));
		}
		double d = [&]() -> double {
			for (auto v : {prv[i], nxt[i]}) {
				if (v == -1) continue;
				int64_t a = get_dist(gems[v].first, gems[v].second);
				int64_t b = max_r;
				int64_t c = sq(gems[i].first - gems[v].first);
				if (a > b + c) {
					double area = std::sqrt(2 * (double(a) * double(b) + double(b) * double(c) + double(c) * double(a)) - double(a) * double(a) - double(b) * double(b) - double(c) * double(c));
					return area / std::abs(gems[i].first - gems[v].first);
				}
			}
			return std::sqrt(max_r);
		}();
		std::cout << gems[i].second+1 << ' ' << std::fixed << std::setprecision(20) << -d << '\n';
	}

	return 0;
}
