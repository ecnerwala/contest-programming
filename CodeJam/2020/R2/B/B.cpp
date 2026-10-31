#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {

		using i64 = int64_t;
		int V, E; cin >> V >> E;
		vector<pair<i64, i64>> time_pts; time_pts.reserve(V);
		vector<pair<i64, i64>> idx_pts; idx_pts.reserve(V);

		time_pts.emplace_back(0, 0);
		for (int i = 1; i < V; i++) {
			int x; cin >> x;
			if (x > 0) {
				time_pts.emplace_back(x, i);
			} else if (x < 0) {
				idx_pts.emplace_back(-x, i);
			} else assert(false);
		}

		sort(time_pts.begin(), time_pts.end());
		sort(idx_pts.begin(), idx_pts.end());
		vector<pair<i64, i64>> times; times.reserve(V);
		auto time_it = time_pts.begin();
		for (auto idx_pt : idx_pts) {
			while (int(times.size()) < idx_pt.first) {
				assert(time_it != time_pts.end());
				times.push_back(*time_it);
				++time_it;
			}
			times.emplace_back(times[idx_pt.first-1].first+1, idx_pt.second);
		}
		times.insert(times.end(), time_it, time_pts.end());

		vector<i64> dists(V);
		for (auto it : times) {
			dists[it.second] = it.first;
		}

		vector<i64> ans(E);
		for (int e = 0; e < E; e++) {
			int u, v; cin >> u >> v; u--, v--;
			ans[e] = max(i64(1), abs(dists[u] - dists[v]));
		}

		cout << "Case #" << case_num << ": ";
		for (int e = 0; e < E; e++) {
			cout << ans[e] << " \n"[e+1==E];
		}
	}

	return 0;
}
