#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<int> C(M);
	for (int j = 0; j < M; j++) {
		cin >> C[j];
	}

	int Q; cin >> Q;
	vector<int64_t> ans(Q);
	vector<vector<pair<int, int>>> queries(M);

	for (int q = 0; q < Q; q++) {
		int x, y; cin >> x >> y;
		y--;
		queries[y].push_back({x, q});
	}

	auto ceil_div = [&](int64_t a, int64_t b) -> int64_t {
		assert(a >= 0);
		return (a + b - 1) / b;
	};
	
	vector<pair<int64_t, array<int64_t, 3>>> fns;
	for (int i = 0; i < M; i++) {
		if (i == 0) {
			fns.push_back({1, {0, C[i], i}});
		} else {
			assert(!fns.empty());
			while (true) {
				if (C[i] <= fns.back().second[1] + (i - fns.back().second[2]) * (2 * fns.back().first + 1)) {
					// we can delete this guy
					int64_t x = fns.back().first;
					array<int64_t, 3> v = {fns.back().second[0] + (i - fns.back().second[2]) * x * x, C[i], i};
					fns.pop_back();
					if (fns.empty() || (C[i] > fns.back().second[1] + (i - fns.back().second[2]) * (2 * x + 1))) {
						fns.push_back({x, v});
						break;
					} else {
						continue;
					}
				} else {
					// let's try to cut him off at the right point
					// find the first x so that C[i] <= fns.back().second[1] + (i - fns.back().second[2]) * (2 * x + 1)
					int64_t x = ceil_div(C[i] - fns.back().second[1], i - fns.back().second[2]) / 2;
					if (x > N) break; // we can skip the guys that are too big
					fns.push_back({x, {fns.back().second[0] + (x - fns.back().first) * fns.back().second[1] + (i - fns.back().second[2]) * x * x, C[i], i}});
					break;
				}
			}
		}
		for (auto it : queries[i]) {
			int64_t x = it.first;
			int q = it.second;
			int mi = -1;
			int ma = int(fns.size());
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (fns[md].first <= x) {
					mi = md;
				} else {
					ma = md;
				}
			}
			assert(mi != -1);
			ans[q] = fns[mi].second[0] + fns[mi].second[1] * (x - fns[mi].first) + (i - fns[mi].second[2]) * x * x;
		}
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
