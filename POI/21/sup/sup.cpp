#include<bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	using std::cin, std::cout, std::cerr;
	using std::min, std::max, std::swap;

	int N, Q; cin >> N >> Q;
	std::vector<int> queries(Q);
	for (auto& q : queries) cin >> q;

	std::vector<int> depth_cnt(N);
	{
		std::vector<int> depth(N);
		depth_cnt[depth[0]] ++;
		for (int i = 1; i < N; i++) {
			int p; cin >> p; p--;
			depth_cnt[depth[i] = depth[p]+1]++;
		}
	}
	while (depth_cnt.back() == 0) depth_cnt.pop_back();
	int D = int(depth_cnt.size());
	for (int i = D-1; i > 0; i--) {
		depth_cnt[i-1] += depth_cnt[i];
	}
	struct constraint_t {
		int d, cnt;
	};
	std::vector<std::pair<constraint_t, int>> hull; hull.reserve(D);
	hull.push_back({{0, depth_cnt[0]}, 1});
	for (int d = 1; d < D; d++) {
		while (true) {
			int k = (hull.back().first.cnt - depth_cnt[d]) / (d - hull.back().first.d);
			if (k < hull.back().second) {
				hull.pop_back();
				continue;
			} else {
				hull.push_back({{d, depth_cnt[d]}, k});
				break;
			}
		}
	}

	assert(hull.back().second <= N);

	std::vector<int> ans(N+1);
	for (int k = 1, j = 0; k <= N; k++) {
		while (j+1 < int(hull.size()) && k > hull[j+1].second) j++;
		ans[k] = hull[j].first.d + (hull[j].first.cnt + k-1) / k;
	}

	for (int q = 0; q < Q; q++) {
		int k = queries[q];
		int res;
		if (k > N) {
			res = hull.back().first.d + (hull.back().first.cnt + k-1) / k;
		} else {
			res = ans[k];
		}
		cout << res << " \n"[q+1==Q];
	}

	return 0;
}
