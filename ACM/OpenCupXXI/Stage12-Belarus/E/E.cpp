#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	struct constraint {
		int l, r; int64_t m;
	};
	vector<constraint> Q(K);
	vector<vector<int>> starts(N);
	vector<vector<int>> ends(N);
	for (int k = 0; k < K; k++) {
		cin >> Q[k].l >> Q[k].r >> Q[k].m; Q[k].l--;
		starts[Q[k].l].push_back(k);
		ends[Q[k].r - 1].push_back(k);
	}

	vector<bool> ans(K, true);
	for (int b = 0; b < 60; b++) {
		// First, build up the array of good/bad
		vector<int> cover_cnt(N);
		vector<int> cover_guy(N);
		{
			int cur_cnt = 0;
			int cur_bad = 0;
			for (int i = 0; i < N; i++) {
				for (int k : starts[i]) {
					if (!((Q[k].m >> b) & 1)) continue;
					cur_cnt++;
					cur_bad ^= k;
				}
				cover_cnt[i] = cur_cnt;
				cover_guy[i] = cur_bad;
				for (int k : ends[i]) {
					if (!((Q[k].m >> b) & 1)) continue;
					cur_cnt--;
					cur_bad ^= k;
				}
			}
		}

		vector<int> uncovered_pref(N+1);
		uncovered_pref[0] = 0;

		vector<pair<int, int>> single_covers; single_covers.reserve(K);
		vector<int> single_cover_cnt(N+1);
		vector<int> single_cover_idx(N+1);

		for (int i = 0; i < N; i++) {
			uncovered_pref[i+1] = uncovered_pref[i] + (cover_cnt[i] == 0);
			single_cover_cnt[i+1] = single_cover_cnt[i] + (cover_cnt[i] == 1);
			if (cover_cnt[i] == 1) {
				if (!single_covers.empty() && single_covers.back().first == cover_guy[i]) {
					single_covers.back().second = i;
				} else {
					single_covers.push_back({cover_guy[i], i});
				}
			}
			single_cover_idx[i+1] = int(single_covers.size());
		}

		int num_bad = 0;
		int bad_id = 0;

		int need_l = 0;
		int need_r = int(single_covers.size()) - 1;

		for (int k = 0; k < K; k++) {
			if ((Q[k].m >> b) & 1) continue;
			// always good
			if (uncovered_pref[Q[k].r] - uncovered_pref[Q[k].l] > 0) continue;
			num_bad++;
			bad_id ^= k;
			if (single_cover_cnt[Q[k].r] - single_cover_cnt[Q[k].l] == 0) {
				// always bad
				need_r = -1;
			} else {
				// find the first single and last single cover
				int i1 = single_cover_idx[Q[k].l];
				if (i1 > 0 && Q[k].l <= single_covers[i1-1].second) i1--;
				int i2 = single_cover_idx[Q[k].r]-1;
				assert(i1 <= i2);
				need_l = max(need_l, i1);
				need_r = min(need_r, i2);
			}
		}

		if (num_bad == 0) continue;
		// otherwise, it has to be one of the things in single cover or bad_id if num_bad == 1
		vector<bool> nans(K, false);
		if (num_bad == 1) {
			nans[bad_id] = ans[bad_id];
		}
		for (int i = need_l; i <= need_r; i++) {
			int k = single_covers[i].first;
			nans[k] = ans[k];
		}
		ans = std::move(nans);
	}

	for (int k = 0; k < K; k++) { cout << ans[k]; } cout << '\n';

	return 0;
}
