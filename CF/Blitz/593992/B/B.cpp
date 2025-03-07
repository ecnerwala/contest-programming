#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::array<int, 2>> C(N);
	for (auto& [x, y] : C) cin >> x >> y;
	std::map<int, int> cnt_home;
	for (auto [x, y] : C) cnt_home[x]++;
	for (auto [x, y] : C) {
		int a = N-1, b = N-1;
		int num_collision = cnt_home[y] - (y == x);
		b -= num_collision;
		a += num_collision;
		cout << a << ' '  << b << '\n';
	}

	return 0;
}
