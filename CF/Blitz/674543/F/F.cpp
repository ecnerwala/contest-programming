#include <bits/stdc++.h>
int main() {
	std::ios-base::sync_with_stdio(false); std::cin.tie(nullptr);
	int N; std::cin >> N;
	std::vector<int> F(N); for (auto& f : F) { std::cin >> f; f--; }

	// >= length of period, and multiple of cycle length
	std::vector<int> dist(N, -1);
	int64_t ans = 1;
	auto dfs = [&](this auto&& self, int i, int depth) -> int {
		if (dist[i] >= 0) return dist[i];
		if (dist[i] == -1) {
			dist[i] = -2;
			int nd = self(F[i], depth+1);
			if (dist[i] < -1) dist[i] = nd+1;
		} else {
			int cnt = 0;
			int j = i;
			do {
				j = F[j];
				dist[j] = 0;
				cnt++;
			} while (j != i);
			ans = std::gcd(ans, cnt);
		}
	};
	int max_dist = 0;
	for (int i = 0; i < N; i++) {
		dfs(i, 0);
		max_dist = std::max(max_dist, dist[i]);
	}
	ans = (max_dist + ans - 1) / ans * ans;
	std::cout << ans << '\n';
}
