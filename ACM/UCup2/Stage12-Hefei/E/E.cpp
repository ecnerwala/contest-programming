#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::pair<int, int>> xs(N*M);
	std::vector<std::pair<int, int>> ys(N*M);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			int c; cin >> c;
			xs.push_back({c, i});
			ys.push_back({c, j});
		}
	}
	int64_t ans = 0;
	for (int z = 0; z < 2; z++, std::swap(xs, ys)) {
		std::sort(xs.begin(), xs.end());
		int cnt = 0;
		int64_t sum = 0;
		for (int i = 0; i < N*M; i++) {
			if (i == 0 || xs[i].first != xs[i-1].first) {
				cnt = 0;
				sum = 0;
			}
			cerr << xs[i].first << ' ' << xs[i].second << '\n';
			ans += 2 * (cnt * int64_t(xs[i].second) - sum);
			cnt++;
			sum += xs[i].second;
		}
	}
	cout << ans << '\n';

	return 0;
}
