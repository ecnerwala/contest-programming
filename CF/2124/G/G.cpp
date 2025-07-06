#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

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
		std::vector<int> mins = A;
		for (int i = 1; i < N; i++) {
			mins[i] = std::min(mins[i], mins[i-1]);
		}

		int64_t base_ans = std::accumulate(mins.begin(), mins.end(), int64_t(0));
		// moving N-1 -> 0 is already base_ans
		std::vector<int64_t> ans(N, base_ans);

		std::vector<std::pair<int, int>> best_remove; best_remove.reserve(N);
		std::vector<int64_t> suff_val(N+1);
		for (int i = N-1, j = N; i >= 0; i--) {
			if (mins[i] == A[i]) {
				assert(N >= 2);
				int cap = i == 0 ? A[0] + best_remove.back().first : mins[i-1];
				// i..j
				for (int k = i; k < j; k++) {
					assert(mins[k] == A[i]);
					mins[k] = (k == i ? cap : std::min(mins[k-1], A[k]));
				}
				for (int k = j-1; k >= i; k--) {
					suff_val[k] = suff_val[k+1] + mins[k];
				}
				{
					int k = j;
					int64_t tot_gain = 0;
					for (auto [v_incr, pos_incr] : best_remove) {
						assert(pos_incr > i);
						int cur_cap = std::min(cap, A[i] + v_incr);
						while (k > i && mins[k-1] <= cur_cap) {
							k--;
							tot_gain += mins[k] - A[i];
						}
						int64_t cnd = base_ans + int64_t(cur_cap - A[i]) * int64_t(k - i) + tot_gain - suff_val[pos_incr];
						int cost = pos_incr - i;
						setmax(ans[cost], cnd);

						if (cur_cap >= cap) break;
					}
				}

				for (int k = j-1; k >= i; k--) {
					mins[k] = A[i];
					suff_val[k] = suff_val[k+1] + mins[k];
				}
				j = i;
			}
			if (best_remove.empty() || A[i] > best_remove.back().first) {
				best_remove.push_back({A[i], i});
			}
		}
		for (int i = N-1; i > 0; i--) {
			setmax(ans[i-1], ans[i]);
		}
		for (int i = 0; i < N; i++) {
			cout << ans[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
