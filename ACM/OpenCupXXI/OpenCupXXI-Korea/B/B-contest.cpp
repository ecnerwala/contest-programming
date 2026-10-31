#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int NA, NB; cin >> NA >> NB;
	vector<int64_t> A(NA); for (auto& a : A) cin >> a;
	vector<int64_t> B(NB); for (auto& b : B) cin >> b;

	/*
	mt19937 mt(48);
	const int NA = 200000;
	const int NB = 170000;
	uniform_int_distribution<int> dist(-int(1e9),int(1e9));
	vector<int64_t> A(NA); for (auto& a : A) a = dist(mt);
	vector<int64_t> B(NB); for (auto& b : B) b = dist(mt);

	reverse(A.begin(), A.end());
	reverse(B.begin(), B.end());
	*/

	vector<pair<int, int>> bad_starts;
	vector<pair<int, int>> bad_ends;

	for (int z = 0; z < 2; z++) {
		vector<pair<int64_t, int>> pref_max_b; pref_max_b.reserve(NB);
		for (int j = 0; j < NB; j++) {
			if (pref_max_b.empty() || B[j] > pref_max_b.back().first) {
				pref_max_b.push_back({B[j], j});
			}
		}

		vector<int64_t> pref_min_b = B;
		for (int j = 1; j < NB; j++) {
			pref_min_b[j] = min(pref_min_b[j], pref_min_b[j-1]);
		}

		vector<pair<int64_t, int>> suff_max_a; suff_max_a.reserve(NA);

		for (int i = 0; i < NA; i++) {
			while (!suff_max_a.empty() && suff_max_a.back().first <= A[i]) {
				suff_max_a.pop_back();
			}
			suff_max_a.push_back({A[i], i});

			auto jt = lower_bound(pref_max_b.begin(), pref_max_b.end(), pair<int64_t, int>{-A[i], -1});
			int j;
			if (jt == pref_max_b.end()) {
				// full wall
				j = NB-1;
			} else {
				assert(jt->first + A[i] >= 0);
				j = jt->second-1;
			}

			if (j != -1) {
				int64_t worst_B = pref_min_b[j];
				assert(worst_B + A[i] < 0);
				// it->first < -worstB
				auto it = upper_bound(suff_max_a.begin(), suff_max_a.end(), pair<int64_t, int>({-worst_B, -1}), std::greater<>());
				assert(it != suff_max_a.end());
				assert(worst_B + it->first < 0);

				int bad_ind;
				if (it == suff_max_a.begin()) {
					// everything is bad
					bad_ind = 0;
				} else {
					--it;
					bad_ind = it->second + 1;
				}
				bad_starts.push_back({bad_ind, i});
			}
		}

		reverse(A.begin(), A.end());
		reverse(B.begin(), B.end());
		swap(bad_starts, bad_ends);
	}
	for (auto& it : bad_ends) {
		swap(it.first, it.second);
		it.first = NA - it.first;
		it.second = NA - it.second;
	}

	/*
	cerr << "Bad starts" << '\n';
	for (auto it : bad_starts) {
		cerr << it.first << ' ' << it.second << '\n';
	}
	cerr << "Bad ends" << '\n';
	for (auto it : bad_ends) {
		cerr << it.first << ' ' << it.second << '\n';
	}
	*/

	const int S = NA;
	struct seg_node {
		int num_unmarked = 1;
		int marks = 0;
	};
	vector<seg_node> seg(2*S);
	auto update_node = [&](int a) {
		if (seg[a].marks > 0) {
			seg[a].num_unmarked = 0;
		} else if (a >= S) {
			seg[a].num_unmarked = 1;
		} else {
			seg[a].num_unmarked = seg[2*a].num_unmarked + seg[2*a+1].num_unmarked;
		}
	};
	for (int i = 2*S-1; i > 0; i--) update_node(i);
	auto update_all_changed = [&](int a) {
		if (!a) return;
		a >>= __builtin_ctz(a);
		a >>= 1;
		for (; a; a >>= 1) {
			update_node(a);
		}
	};

	auto mark_range = [&](int l, int r, int v) {
		assert(0 <= l && l <= r && r <= S);
		for (int a = S+l, b = S+r; a < b; a >>= 1, b >>= 1) {
			if (a & 1) {
				seg[a].marks += v;
				update_node(a);
				a++;
			}
			if (b & 1) {
				--b;
				seg[b].marks += v;
				update_node(b);
			}
		}
		update_all_changed(S+l);
		update_all_changed(S+r);
	};

	for (int i = 0; i < NA; i++) {
		mark_range(i, i+1, +1);
	}

	vector<bool> end_good(NA);
	{
		vector<int> bad_ends_cnt(NA+1);
		for (auto it : bad_ends) {
			bad_ends_cnt[it.first-1]++;
			bad_ends_cnt[it.second]--;
		}
		for (int i = 0; i < NA; i++) {
			end_good[i] = bad_ends_cnt[i] == 0;
			bad_ends_cnt[i+1] += bad_ends_cnt[i];
		}
	}
	for (auto it : bad_starts) {
		mark_range(it.first, it.second+1, +1);
	}

	int64_t ans = 0;
	{
		int64_t worst_B = *min_element(B.begin(), B.end());
		int64_t best_B = *max_element(B.begin(), B.end());
		int last_open = -1;
		for (int i = 0; i < NA; i++) {
			mark_range(i, i+1, -1);
			if (A[i] + best_B < 0) {
				// all of these are bad
				mark_range(0, i+1, +1);
			}
			if (A[i] + worst_B >= 0) {
				last_open = i;
			}
			if (end_good[i]) {
				mark_range(last_open+1, i+1, +1);
				ans += seg[1].num_unmarked;
				mark_range(last_open+1, i+1, -1);
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
