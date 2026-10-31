#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::array<int, 2>> ans; ans.reserve(3 * N + 3 * M);

	std::array<int, 8> nxt_val;
	for (int i = 0; i < 8; i++) nxt_val[i] = (i * 3) % 8;
	for (int a = 0; a <= N + 3 * M; a++) {
		while (true) {
			int b = nxt_val[a & 7];
			assert((3*b-a) % 8 == 0);
			int x = (3 * b - a) / 8;
			assert((3*a-b) % 8 == 0);
			int y = (3 * a - b) / 8;
			if (x >= N || y < 0) break;
			nxt_val[a & 7] += 8;
			if (x < 0 || y >= M) continue;
			ans.push_back({x, y});
			break;
		}
	}
	cout << ans.size() << '\n';
	for (auto [x, y] : ans) {
		cout << x+1 << ' ' << y+1 << '\n';
	}

	return 0;
}
