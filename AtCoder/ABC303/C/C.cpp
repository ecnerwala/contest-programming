#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, H, K; cin >> N >> M >> H >> K;
	string S; cin >> S;
	std::set<std::pair<int, int>> items;
	for (int k = 0; k < M; k++) {
		int x, y; cin >> x >> y;
		items.insert({x, y});
	}

	int x = 0, y = 0;
	int h = H;
	for (int i = 0; i < N; i++) {
		char c = S[i];
		if (c == 'L') {
			x--;
		} else if (c == 'R') {
			x ++;
		} else if (c == 'U') {
			y++;
		} else if (c == 'D') {
			y--;
		} else assert(false);

		h--;
		if (h < 0) {
			cout << "No" << '\n';
			exit(0);
		}
		if (h < K && items.count({x, y})) {
			items.erase({x, y});
			h = K;
		}
	}
	cout << "Yes" << '\n';

	return 0;
}
