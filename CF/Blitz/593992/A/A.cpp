#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::array<int, 2>> P(N);
	for (auto& [x, y] : P) cin >> x >> y;
	int cnt = 0;
	for (int i = 0; i < N; i++) {
		auto [x, y] = P[i];
		int has = 0;
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			auto [x2, y2] = P[j];
			if (y == y2) {
				if (x > x2) has |= 1;
				else if (x < x2) has |= 2;
			} else if (x == x2) {
				if (y > y2) has |= 4;
				else if (y < y2) has |= 8;
			}
		}
		if (has == 15) cnt++;
	}
	cout << cnt << '\n';

	return 0;
}
