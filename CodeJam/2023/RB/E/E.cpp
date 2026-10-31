#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<int> D(N);
		for (auto& d : D) { cin >> d; d--; }
		std::vector<int> C(N);
		for (auto& c : C) cin >> c;

		std::vector<int64_t> ch_sum(N);
		for (int i = 0; i < N; i++) {
			ch_sum[D[i]] += C[i];
		}

		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			ans += std::max<int64_t>(C[i] - ch_sum[i], 0);
		}

		std::vector<int> vis(N, -1);
		for (int i = 0; i < N; i++) {
			int cur = i;
			while (vis[cur] == -1) {
				vis[cur] = i;
				cur = D[cur];
			}
			if (vis[cur] != i) continue;
			// cycle starting at cur
			int st = cur;
			int64_t best_diff = 1e18;
			do {
				best_diff = std::min(best_diff,
					std::max<int64_t>(C[D[cur]] - (ch_sum[D[cur]] - C[cur]), 0)
					- std::max<int64_t>(C[D[cur]] - ch_sum[D[cur]], 0));
				cur = D[cur];
			} while (cur != st);
			ans += best_diff;
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
