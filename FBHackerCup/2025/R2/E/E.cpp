#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; int64_t M; cin >> N >> M;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;

		for (int i = 1; i < N; i++) A[i] += A[i-1];

		int best_idx = 1;
		for (int i = 2; i <= N; i++) {
			if (A[i-1] * best_idx > A[best_idx-1] * i) {
				best_idx = i;
			}
		}

		cerr << "Case #" << case_num << ": " << N << ' ' << best_idx << '\n';
		std::vector<int64_t> X(N, 0);

		int64_t num_best = (M + A[best_idx - 1] - 1) / A[best_idx - 1];
		if (num_best > N) {
			// do the dijkstra
			std::vector<int64_t> cost(best_idx);
			std::vector<int> prv(best_idx, -1);
			cost[0] = 0;
			if (1 < best_idx) {
				for (int j = 1; j < best_idx; j++) {
					cost[j] = cost[j-1] + A[0];
					prv[j] = 1;
				}
			}
			for (int d = 2; d <= N; d++) {
				for (int i = 0; i < best_idx; i++) {
					int64_t v = cost[i] + A[d-1];
					int j = i + d;
					v -= (j / best_idx) * A[best_idx - 1];
					j %= best_idx;
					// strictly less so no cycles
					if (v > cost[j]) {
						cost[j] = v;
						prv[j] = d;
					}
				}
			}
			assert(cost[0] == 0);
			std::pair<int, int> best_st;
			for (int i = 0; i < best_idx; i++) {
				assert(cost[i] <= M);
				int64_t cnd = (M - cost[i] + A[best_idx - 1] - 1) / (A[best_idx - 1]);
				cnd *= best_idx;
				cnd += i;
				if (i == 0 || cnd < best_st.first) {
					best_st = {cnd, i};
				}
			}
			{
				int i = best_st.second;
				int64_t v = 0;
				while (i != 0) {
					int d = prv[i];
					X[d]++;
					v += A[d-1];
					i -= d;
					i %= best_idx;
					if (i < 0) i += best_idx;
				}
				assert(v <= M);
				X[best_idx] = (M - v + A[best_idx-1] - 1) / A[best_idx - 1];
				assert(std::accumulate(X.begin(), X.end(), int64_t(0)) == best_st.first);
			}
		} else {
			std::vector<int64_t> dp(N*N+1);
			std::vector<int> prv(N*N+1);
			dp[0] = 0, prv[0] = -1;
			for (int k = 1; k <= N*N; k++) {
				int64_t v = 0;
				for (int i = 1; i <= N && i <= k; i++) {
					v = std::max(v, dp[k-i] + A[i-1]);
				}
			}
		}

		for (int i = N-1; i > 0; i--) X[i-1] += X[i];
		cout << "Case #" << case_num << ": ";
		for (int i = 0; i < N; i++) cout << X[i] << " \n"[i+1==N];
		cout << flush;
	}

	return 0;
}
