#include<bits/stdc++.h>
#include <numeric>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		vector<int64_t> E(N);
		vector<pair<int64_t, int>> R(N);
		for (int i = 0; i < N; i++) {
			cin >> E[i] >> R[i].first;
			R[i].first += E[i];
			R[i].second = i;
		}
		sort(R.begin(), R.end());

		int64_t C = std::accumulate(E.begin(), E.end(), int64_t(0));

		// compute prefix sums of alive toys
		vector<int64_t> bit(N);
		for (int i = 0; i < N; i++) {
			for (int a = i; a < N; a |= a+1) {
				bit[a] += E[i];
			}
		}

		priority_queue<int, vector<int>, greater<int>> is_bad;

		const int64_t INF = 8e18;
		int64_t ans = -1;
		int best_remove = -1;
		for (int z = 0; z < N; z++) {
			while (!R.empty() && C < R.back().first) {
				is_bad.push(R.back().second);
				R.pop_back();
			}

			if (is_bad.empty()) {
				ans = INF;
				best_remove = z;
				break;
			}

			int cur = is_bad.top(); is_bad.pop();
			int64_t cnd = C;
			for (int a = cur; a > 0; a &= a-1) {
				cnd += bit[a-1];
			}
			if (cnd > ans) {
				ans = cnd;
				best_remove = z;
			}

			C -= E[cur];
			for (int a = cur; a < N; a |= a+1) {
				bit[a] -= E[cur];
			}
		}

		cout << "Case #" << case_num << ": ";
		cout << best_remove << ' ';
		if (ans == INF) cout << "INDEFINITELY" << '\n';
		else cout << ans << '\n';
	}

	return 0;
}

// Let C = sum_chosen E_i
// Bad toy is C < E_i + R_i
// Cry time = first time you revisit a toy with C < E_i + R_i
//          = C + sum(earlier E_i)
