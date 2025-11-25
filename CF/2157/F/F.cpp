#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::pair<int, int>> ops;
	int best_D = -1;
	int best_cost = int(1e6) + 1;
	int max_w = 0;
	int max_w_cnt = 0;
	for (int D = 1; D <= N; D++) {
		if (D < 30 && (1 << D) < N) continue;
		int rem = N;
		int64_t cnt = 1;
		int cost = (D-1) * 1000;
		if (cost >= best_cost) break;
		int w = 0;
		while (rem) {
			if (w) {
				cnt *= (D + w - 1);
				assert(cnt % w == 0);
				cnt /= w;
			}
			if (cnt > rem) cnt = rem;
			cost += int(cnt) * w;
			if (cnt == rem) break;
			rem -= cnt;
			w++;
		}
		if (cost < best_cost) {
			best_cost = cost;
			best_D = D;
			max_w = w;
			max_w_cnt = rem;
		}
	}
	cerr << best_D << '\n';
	cerr << best_cost << '\n';
	int D = best_D;
	{
		std::vector<std::vector<int>> arrs; arrs.reserve(N);
		for (int w = 0; int(arrs.size()) < N; w++) {
			std::vector<int> cur(D, 0);
			cur[0] = w;
			do {
				arrs.push_back(cur);
				if (int(arrs.size()) == N) break;
			} while (true);
		}
	}

	std::vector<std::vector<std::pair<int, int>>> layer_ops(D);
	[&](this auto&& self, int cur_layer, int cur_w, int next) -> int {
		assert(next >= 0);
		if (cur_w == max_w) {
			// optimization
			cur_layer = D;
		}
		if (cur_layer == D) {
			if (cur_w == max_w) {
				max_w_cnt--;
				if (max_w_cnt == 0) {
					max_w--;
					// just let max_w_cnt go negative
				}
			}
			return next-1;
		}
		int tgt = next;
		for (int i = max_w - cur_w; i >= 0 && next >= 0; i--) {
			if (next != tgt) layer_ops[cur_layer].push_back({next, tgt - next});
			next = self(cur_layer + 1, cur_w + i, next);
		}
		return next;
	}(0, 0, N-1);
	for (int d = D-1; d >= 0; d--) {
		ops.insert(ops.end(), layer_ops[d].begin(), layer_ops[d].end());
	}

	cout << ops.size() << '\n';
	for (auto [x, l] : ops) cout << x+1 << ' ' << l << '\n';

	int cost = 0;
	for (int i = 0; i < int(ops.size()); i++) {
		cost += ops[i].second;
		if (i && ops[i].first > ops[i-1].first) cost += 1000;
	}
	assert(cost == best_cost);
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
