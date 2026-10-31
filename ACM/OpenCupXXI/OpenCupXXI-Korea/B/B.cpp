#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int NA, NB; cin >> NA >> NB;
	vector<int64_t> A(NA); for (auto& a : A) cin >> a;
	vector<int64_t> B(NB); for (auto& b : B) cin >> b;

	vector<bool> start_good(NA);
	vector<bool> end_good(NA);

	for (int z = 0; z < 2; z++) {
		vector<pair<int64_t, int64_t>> pref_max_b; pref_max_b.reserve(NB);
		{
			int64_t cur_min = B[0];
			int64_t cur_max = B[0];
			for (int j = 1; j < NB; j++) {
				if (B[j] > cur_max) {
					pref_max_b.push_back({B[j], cur_min});
					cur_max = B[j];
				} else if (B[j] < cur_min) {
					cur_min = B[j];
				}
			}
			pref_max_b.push_back({0, cur_min});
		}

		vector<pair<int64_t, int>> suff_max_a; suff_max_a.reserve(NA);

		for (int i = 0; i < NA; i++) {
			if (A[i] + B[0] < 0) {
				auto jt = lower_bound(pref_max_b.begin(), pref_max_b.end()-1, -A[i], [](pair<int64_t, int64_t> a, int64_t b) { return a.first < b; });
				int64_t worst_B = jt->second;
				assert(worst_B + A[i] < 0);
				// They all get marked as bad, so we can just pop them
				while (!suff_max_a.empty() && suff_max_a.back().first + worst_B < 0) {
					suff_max_a.pop_back();
				}
			} else {
				suff_max_a.push_back({A[i], i});
			}
		}

		for (auto [_, v] : suff_max_a) {
			start_good[v] = true;
		}

		reverse(A.begin(), A.end());
		reverse(B.begin(), B.end());
		swap(start_good, end_good);
	}
	reverse(end_good.begin(), end_good.end());

	int64_t ans = 0;
	{
		int64_t worst_B = *min_element(B.begin(), B.end());
		int64_t best_B = *max_element(B.begin(), B.end());
		int nxt_open = 0;
		int cur_cnt = 0;
		for (int i = 0; i < NA; i++) {
			if (A[i] + best_B < 0) {
				nxt_open = i+1;
				cur_cnt = 0;
			} else if (A[i] + worst_B >= 0) {
				while (nxt_open <= i) {
					cur_cnt += start_good[nxt_open++];
				}
			}
			if (end_good[i]) {
				ans += cur_cnt;
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
