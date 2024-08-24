#include <bits/stdc++.h>

int main() {
	using namespace std;
	//ios_base::sync_with_stdio(false), cin.tie(nullptr);

	if (fopen("king.in", "r")) {
		freopen("king.in", "r", stdin);
		freopen("king.out", "w", stdout);
	}

	int D, M, Y; cin >> D >> M >> Y;
	// born before this
	std::tuple<int, int, int, int> king{Y - 18, M, D, int(1e9)};
	std::tuple<int, int, int, int> ans{-1000, -1, -1, -1};
	int N; cin >> N;
	for (int i = 0; i < N; i++) {
		int d, m, y; cin >> d >> m >> y;
		std::tuple<int, int, int, int> cur{y, m, d, i+1};
		if (cur <= king && cur > ans) ans = cur;
	}
	cout << std::get<3>(ans) << '\n';

	return 0;
}
