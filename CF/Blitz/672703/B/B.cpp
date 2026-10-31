#include <bits/stdc++.h>

int main() {
	int64_t B, D, S; std::cin >> B >> D >> S;
	int64_t ans = B + D + S;
	if (B + S == 0 && D == 1) ans = 0;
	auto check = [&](int a, int b, int c) -> void {
		if (B < a || D < b || S < c) return;
		int64_t x = B - a, y = D - b, z = S - c;
		int64_t cnd = 3 * std::max({int64_t(0), x, y, z}) - x - y - z;
		ans = std::min(ans, cnd);
	};
	check(0, 0, 0);
	check(1, 0, 0);
	check(1, 1, 0);
	check(0, 1, 0);
	check(0, 0, 1);
	check(0, 1, 1);
	check(1, 0, 1);
	std::cout << ans << '\n';
}
