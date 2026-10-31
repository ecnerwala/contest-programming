#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<std::pair<int, int>> a; a.reserve(100 * 100);
	int M; cin >> M;
	for (int i = 0; i < M; i++) {
		int d; cin >> d;
		for (int j = 0; j < d; j++) {
			a.push_back({i, j});
		}
	}
	auto [x, y] = a[int(a.size()) / 2];
	cout << x+1 << ' ' << y+1 << '\n';

	return 0;
}
