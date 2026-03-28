#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> cost(N);
		std::vector<int64_t> good_cost(N);
		for (int cur = N-1; cur >= 0; cur--) {
			int64_t a; std::cin >> a;
			int num_ch; std::cin >> num_ch;
			good_cost[cur] = 1;
			for (int z = 0; z < num_ch; z++) {
				int nxt; std::cin >> nxt; nxt--;
				cost[cur] += cost[nxt];
				good_cost[cur] = std::lcm(good_cost[cur], std::gcd(a, good_cost[nxt]));
			}
			if (good_cost[cur] == 1) {
				cost[cur]++;
				good_cost[cur] = a;
			}
			std::cout << cost[cur] << '\n' << std::flush;
		}
	}

	return 0;
}
