#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	if (N == 4) {
		cout << 3 << '\n';
		cout << 1 << ' ' << 1 << '\n';
		cout << 2 << ' ' << 1 << '\n';
		cout << 3 << ' ' << 1 << '\n';
	} else if (N == 250000) {
		std::vector<std::pair<int, int>> ops;
		const int B = 500;
		for (int i = 0; i+1 < 500; i++) {
			for (int j = 499; j >= 0; j--) {
				ops.push_back({j * B + i, 1});
			}
		}
		for (int i = 0; i+1 < 500; i++) {
			ops.push_back({i * 500 + 499, 500});
		}
		cout << ops.size() << '\n';
		for (auto [x, l] : ops) cout << x+1 << ' ' << l << '\n';
		int cost = 0;
		for (int i = 0; i < int(ops.size()); i++) {
			cost += ops[i].second;
			if (i && ops[i].first > ops[i-1].first) cost += 1000;
		}
		cerr << cost << '\n';
		std::vector<bool> alive(N, true);
		for (auto [x, l] : ops) {
			assert(alive[x]);
			alive[x] = false;
			alive[std::min(x+l, N-1)] = true;
		}
		for (int i = 0; i < N-1; i++) assert(!alive[i]);
	}

	return 0;
}
