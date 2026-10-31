#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);

	int N, X; std::cin >> N >> X;
	std::vector<std::array<int, 2>> segs(N);
	int cur_time = 0;
	int ans = 0;
	for (auto& [l, r] : segs) {
		std::cin >> l >> r;
		while (cur_time + X <= l) {
			ans ++;
			cur_time += X;
		}
		ans += r - cur_time;
		cur_time = r+1;
	}
	std::cout << ans << '\n';
}
