#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	vector<pair<int64_t, int>> H(N);
	for (int i = 0; i < N; i++) {
		cin >> H[i].first;
		H[i].second = i;
	}

	vector<pair<array<int, 2>, int>> queries(Q);
	for (int q = 0; q < Q; q++) {
		int s, e; cin >> s >> e; s--;
		queries[q] = {{s, e}, q};
	}
	vector<int64_t> ans(Q, 0);

	for (int dir = 0; dir < 2; dir++) {
		if (dir) {
			reverse(H.begin(), H.end());
			for (auto& q : queries) {
				q.first = {N - q.first[1], N - q.first[0]};
			}
		}
		sort(queries.begin(), queries.end(), [](const auto& a, const auto& b) { return a.first[1] < b.first[1]; });

		struct dat_t {
			int idx;
			int64_t height;
			int block_sz;
			int64_t tot_water;
		};
		vector<dat_t> stk; stk.reserve(N);
		stk.push_back(dat_t{-1, 0, 0, 0});

		auto qit = queries.begin();
		for (int i = 0; i < N; i++) {

			dat_t cur_blk;
			cur_blk.idx = i;
			cur_blk.height = H[i].first;
			cur_blk.block_sz = 1;
			cur_blk.tot_water = stk.back().tot_water;

			while (stk.back().idx != -1 && H[i] > H[stk.back().idx]) {
				cur_blk.block_sz += stk.back().block_sz;
				cur_blk.tot_water += stk.back().block_sz * (cur_blk.height - stk.back().height);
				stk.pop_back();
			}

			stk.push_back(cur_blk);

			for (; qit != queries.end() && qit->first[1] == i+1; ++qit) {
				int l = qit->first[0];
				assert(l <= i);
				int q = qit->second;
				int mi = 0, ma = int(stk.size());
				while (ma - mi > 1) {
					int md = mi + (ma - mi) / 2;
					if (stk[md].idx < l) mi = md;
					else ma = md;
				}
				assert(ma != int(stk.size()) && l <= stk[ma].idx);
				int64_t val = stk.back().tot_water - stk[ma].tot_water;
				ans[q] += val;
			}
		}
		assert(qit == queries.end());
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
