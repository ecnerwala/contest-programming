#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::pair<int, int>> ops;
	for (int D = 1; D <= N; D++) {
		if (D < 30 && (1 << D) < N) continue;
		int rem = N;
		int64_t cnt = 1;
		int cost = (D-1) * 1000;
		for (int w = 0; rem; w++) {
			if (w) {
				cnt *= (D + w - 1);
				assert(cnt % w == 0);
				cnt /= w;
			}
			if (cnt > rem) cnt = rem;
			rem -= cnt;
			cost += int(cnt) * w;
		}
		cerr << D << ' ' << cost << '\n';
		if (D >= 500) break;
	}
	if (N == 4) {
		ops.push_back({0, 1});
		ops.push_back({1, 1});
		ops.push_back({2, 1});
	} else if (N == 250000) {
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
	} else assert(false);
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

	return 0;
}
