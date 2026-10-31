#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) {
			cin >> a;
		}
		std::vector<int> pref = A;
		std::vector<int> suff = A;
		for (int i = 0; i+1 < N; i++) {
			pref[i+1] &= pref[i];
		}
		for (int i = N-1; i-1 >= 0; i--) {
			suff[i-1] &= suff[i];
		}
		int ans = 0;
		for (int i = 0; i+1 < N; i++) {
			ans = std::max(ans, pref[i] + suff[i+1]);
		}
		int msk_2 = 0;
		int msk_1 = 0;

		struct split_dat {
			bool both_ones = false;
			std::optional<int> l;
			std::optional<int> r;
			std::vector<std::pair<int, int>> fixed;
		};
		std::vector<split_dat> splits(N-1);
		for (int s = 0; s < N-1; s++) {
			splits[s].both_ones = true;
		}
		std::vector<bool> is_ones(N);
		for (int b = 29; b >= 0; b--) {
			std::vector<int> zero_loc; zero_loc.reserve(N);
			std::vector<int> all_ones_cnt(N+1);
			for (int i = 0; i < N; i++) {
				is_ones[i] = (A[i] & msk_1) == msk_1;
				all_ones_cnt[i+1] = all_ones_cnt[i];
				if ((A[i] & (1 << b)) && is_ones[i]) {
					all_ones_cnt[i+1]++;
				}
				if (!(A[i] & (1 << b))) {
					zero_loc.push_back(i);
				}
			}
			int cnt_0 = int(zero_loc.size());
			if (cnt_0 == 0) {
				msk_2 |= (1 << b);
				msk_1 |= (1 << b);
				continue;
			}
			if (cnt_0 == 1) {
				msk_2 |= (1 << b);
				continue;
			}
			assert(cnt_0 >= 2);
			bool can_do = false;
			std::vector<split_dat> nsplits(N-1);
			for (int s = 0; s < N-1; s++) {
				bool suff_has_all_ones = all_ones_cnt[N] > all_ones_cnt[s+1];
				bool pref_has_all_ones = all_ones_cnt[s+1] > all_ones_cnt[0];
				if (zero_loc[0] <= s && s+1 <= zero_loc.end()[-1]) {
					if (zero_loc[1] <= s && s+1 <= zero_loc.end()[-2]) {
						// not good
					} else {
						if (s+1 <= zero_loc[1]) {
							// can we swap zero_loc[0] and some later 1
							if (is_ones[zero_loc[0]]) {
								if (splits[s].both_ones && suff_has_all_ones) {
									nsplits[s].l = {zero_loc[0]};
								}
								if (splits[s].r && ((A[*splits[s].r] >> b) & 1)) {
									nsplits[s].fixed.push_back({zero_loc[0], *splits[s].r});
								}
							} else {
								if (splits[s].l && *splits[s].l == zero_loc[0] && suff_has_all_ones) {
									nsplits[s].l = {zero_loc[0]};
								}
								for (auto [l, r] : splits[s].fixed) {
									if (l == zero_loc[0] && ((A[r] >> b) & 1)) {
										nsplits[s].fixed.push_back({zero_loc[0], r});
									}
								}
							}
						}
						if (s >= zero_loc.end()[-2]) {
							// we can swap zero_loc.back() and some prefix 1
							if (is_ones[zero_loc.back()]) {
								if (splits[s].both_ones && pref_has_all_ones) {
									nsplits[s].r = {zero_loc.back()};
								}
								if (splits[s].l && ((A[*splits[s].l] >> b) & 1)) {
									nsplits[s].fixed.push_back({*splits[s].l, zero_loc.back()});
								}
							} else {
								if (splits[s].r && *splits[s].r == zero_loc.back() && pref_has_all_ones) {
									nsplits[s].r = {zero_loc.back()};
								}
								for (auto [l, r] : splits[s].fixed) {
									if (r == zero_loc.back() && ((A[l] >> b) & 1)) {
										nsplits[s].fixed.push_back({l, zero_loc.back()});
									}
								}
							}
						}
					}
				} else {
					// we're not in the range, so you have to swap 1 for 1
					if (splits[s].both_ones && pref_has_all_ones && suff_has_all_ones) {
						nsplits[s].both_ones = true;
					}
					if (splits[s].l && ((A[*splits[s].l] >> b) & 1) && suff_has_all_ones) {
						nsplits[s].l = splits[s].l;
					}
					if (splits[s].r && ((A[*splits[s].r] >> b) & 1) && pref_has_all_ones) {
						nsplits[s].r = splits[s].r;
					}
					for (auto [l, r] : splits[s].fixed) {
						if (((A[l] >> b) & 1) && ((A[r] >> b) & 1)) {
							nsplits[s].fixed.push_back({l, r});
						}
					}
				}

				if (nsplits[s].both_ones || nsplits[s].l || nsplits[s].r || nsplits[s].fixed.size()>0) {
					can_do = true;
				}
			}

			/*
			cerr << "checking " << b << ' ' << (1 << b) << '\n';
			for (int i = 0; i < N; i++) {
				cerr << A[i] << ' ' << "is_ones=" << is_ones[i] << ' ' << "bit=" << ((A[i] >> b) & 1) << ' ' << all_ones_cnt[i+1] - all_ones_cnt[i] << '\n';
				if (i < N-1) {
					int s = i;
					cerr << "  split " << nsplits[s].both_ones << ' ';
					if (nsplits[s].l) cerr << "l=" << *nsplits[s].l << ' ';
					if (nsplits[s].r) cerr << "r=" << *nsplits[s].r << ' ';
					for (auto [l, r] : nsplits[s].fixed) cerr << "l,r=" << l << ',' << r << ' ';
					cerr << '\n';
				}
			}
			*/
			if (can_do) {
				msk_1 |= (1 << b);
				splits = std::move(nsplits);
			}
		}
		ans = std::max(ans, msk_2 + msk_1);
		cout << ans << '\n';
	}

	return 0;
}
