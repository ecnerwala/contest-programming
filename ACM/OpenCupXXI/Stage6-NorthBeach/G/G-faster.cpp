#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int N, Q; cin >> N >> Q; ) {
		using pt_t = array<int, 2>;
		vector<pt_t> coords(N+1);
		for (int i = 0; i < N; i++) {
			cin >> coords[i][0] >> coords[i][1];
		}
		coords[N] = coords[0];
		vector<pt_t> queries(Q);
		for (int q = 0; q < Q; q++) {
			cin >> queries[q][0] >> queries[q][1];
		}

		// evts by which axis is sweeping
		array<vector<pair<int, pair<bool, int>>>, 2> evts;
		evts[0].reserve(N);
		evts[1].reserve(N);
		for (int i = 0; i < N; i++) {
			pt_t p = coords[i], q = coords[i+1];
			int z = (p[1] != q[1]);
			assert(p[z] != q[z]);
			assert(p[!z] == q[!z]);
			if (p[z] > q[z]) swap(p, q);
			evts[z].push_back({p[z], {0, p[!z]}});
			evts[z].push_back({q[z], {1, q[!z]}});
		}
		for (int z = 0; z < 2; z++) {
			assert(int(evts[z].size()) == N);
			sort(evts[z].begin(), evts[z].end());
		}

		array<vector<tuple<int, pair<int, int>, int>>, 2> qevents;
		qevents[0].reserve(Q);
		qevents[1].reserve(Q);

		vector<int> max_ans(Q, int(1e9));
		{
			qevents[0].clear();
			qevents[1].clear();
			for (int q = 0; q < Q; q++) {
				auto pt = queries[q];
				for (int z = 0; z < 2; z++) {
					qevents[z].push_back({pt[z], {pt[!z], pt[!z]}, q});
				}
			}
			for (int z = 0; z < 2; z++) {
				sort(qevents[z].begin(), qevents[z].end());
				auto it1 = evts[z].begin();
				set<int> has;
				for (auto [x, bounds, q] : qevents[z]) {
					for (; it1 != evts[z].end() && it1->first <= x; ++it1) {
						auto [d, v] = it1->second;
						if (d == 1) {
							//assert(has.find(v) != has.end());
							has.erase(v);
						} else {
							//assert(has.find(v) == has.end());
							has.insert(v);
						}
					}
					auto it = has.upper_bound(bounds.first);
					assert(it != has.end());
					max_ans[q] = min(max_ans[q], *it - bounds.first);
					// max_ans is known to be bad
				}
			}
		}

		vector<int> ans(Q);
		//for (int lg = 1 << 27; lg >= 1; lg /= 2) {
		for (int lg = 1 << 27; lg >= 1; lg /= 2) {
			qevents[0].clear();
			qevents[1].clear();
			for (int q = 0; q < Q; q++) {
				auto pt = queries[q];
				int len = (ans[q] += lg);
				if (ans[q] >= max_ans[q]) {
					ans[q] -= lg;
					continue;
				}
				for (int z = 0; z < 2; z++) {
					qevents[z].push_back({pt[z]+len, {pt[!z], pt[!z]+len}, q});
				}
			}
			for (int z = 0; z < 2; z++) {
				sort(qevents[z].begin(), qevents[z].end());
				auto it1 = evts[z].begin();
				set<int> has;
				for (auto [x, bounds, q] : qevents[z]) {
					for (; it1 != evts[z].end() && it1->first <= x; ++it1) {
						auto [d, v] = it1->second;
						if (d == 1) {
							//assert(has.find(v) != has.end());
							has.erase(v);
						} else {
							//assert(has.find(v) == has.end());
							has.insert(v);
						}
					}
					auto it = has.upper_bound(bounds.first);
					assert(it != has.end());
					if (*it <= bounds.second) {
						ans[q] &= ~lg;
					}
				}
			}
		}

		for (int q = 0; q < Q; q++) {
			cout << (ans[q]+1) << '\n';
		}
	}

	return 0;
}
