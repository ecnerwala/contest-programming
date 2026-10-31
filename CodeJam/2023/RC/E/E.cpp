#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int P; cin >> P;
		string S; cin >> S;
		int N = int(S.size());

		std::vector<bool> split(N+1, false);
		split[0] = split[N] = true;
		std::vector<bool> dont_split(N+1, false);
		auto solve = [&]() -> bool {
			if (P >= 4) {
				for (int i = 0; i+1 < N; i++) {
					if (S[i] > S[i+1]) {
						split[i] = true;
						split[i+1] = true;
						split[i+2] = true;
						return true;
					}
				}
				// Now, it must be weakly increasing
				if (P < N) {
					// If there are 3 in a row, it's bad
					for (int i = 0; i+2 < N; i++) {
						if (S[i] == S[i+1] && S[i] == S[i+2]) {
							split[i] = true;
							dont_split[i+1] = true;
							split[i+2] = true;
							split[i+3] = true;
							return true;
						}
					}
				}
				// Always good
				return false;
			} else if (P == 3) {
				for (int i = 0; i < N; i++) {
					if (S[i] < S[0]) {
						split[i] = true;
						split[i+1] = true;
						return true;
					}
				}
				for (int i = N-1; i >= 0; i--) {
					if (S[i] > S[N-1]) {
						split[i] = true;
						split[i+1] = true;
						return true;
					}
				}
				for (int i = 2; i < N-1; i++) {
					if (S[i] == S[0]) {
						split[i] = true;
						split[i+1] = true;
						return true;
					}
				}

				if (S[0] == S[N-1]) {
					// everything is equal, so 2 >= N-1
					assert(N == 3);
					// nothing to do
					return false;
				}

				int num_after = 1;
				int num_cur = 0;
				for (int i = N-2; i > 0; i--) {
					if (S[i] == S[N-1]) {
						num_cur++;
						if (num_cur > num_after) {
							split[i] = true;
							split[i + num_cur] = true;
							return true;
						}
					} else {
						num_after += num_cur;
						num_cur = 0;
					}
				}
				return false;
			} else if (P == 2) {
				string pref_mins = S;
				string suff_maxs = S;
				for (int i = 0; i+1 < N; i++) {
					pref_mins[i+1] = std::min(pref_mins[i+1], pref_mins[i]);
				}
				for (int i = N-1; i-1 >= 0; i--) {
					suff_maxs[i-1] = std::max(suff_maxs[i-1], suff_maxs[i]);
				}

				int num_first = 0;
				for (int i = 0; i < N; i++) num_first += (S[i] == S[0]);
				for (int i = 1; i < N; i++) {
					if (pref_mins[i-1] < suff_maxs[i]) {
						// he can win
						continue;
					} else if (pref_mins[i-1] > suff_maxs[i]) {
						split[i] = true;
						return true;
					} else {
						assert(pref_mins[i-1] == suff_maxs[i]);
						if (pref_mins[i-1] == suff_maxs[0] && i <= num_first - i) {
							// he can win
							continue;
						} else {
							split[i] = true;
							return true;
						}
					}
				}
				return false;
			} else assert(false);
		};

		auto ans = solve();
		cout << "Case #" << case_num << ": ";
		if (ans) {
			int need_splits = P + 1 - std::accumulate(split.begin(), split.end(), 0);
			for (int i = 0; i <= N; i++) {
				if (need_splits > 0 && !dont_split[i] && !split[i]) {
					split[i] = true;
					need_splits --;
				}
			}
			assert(need_splits == 0);

			cout << "POSSIBLE" << '\n';
			for (int i = 0; i < N; i++) {
				if (i && split[i]) {
					cout << ' ';
				}
				cout << S[i];
			}
			cout << '\n';
		} else {
			cout << "IMPOSSIBLE" << '\n';
		}
	}

	return 0;
}
