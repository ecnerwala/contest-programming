#include <bits/stdc++.h>

int main() {
	int N, K; std::cin >> N >> K;
	std::vector<int> A(N);
	for (auto& a : A) std::cin >> a;
	std::sort(A.begin(), A.end(), [&](int a, int b) -> bool {
		return a % 10 > b % 10;
	});
	for (auto& a : A) {
		if (a == 100) continue;
		int cost = 10 - a % 10;
		assert(a + cost <= 100);
		if (K >= cost) {
			a += cost;
			K -= cost;
		}
	}
	for (auto& a : A) {
		while (a <= 90 && K >= 10) {
			a += 10;
			K -= 10;
		}
	}
	int64_t ans = 0;
	for (auto a : A) {
		ans += a / 10;
	}
	std::cout << ans << '\n';
}
