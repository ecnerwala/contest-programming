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
		int64_t ans;
		std::vector<int64_t> X(N, 0);

		int64_t num_best = (M + A[best_idx - 1] - 1) / A[best_idx - 1];
		std::vector<int64_t> dp(best_idx*N+1, -1);
		std::vector<int> prv(best_idx*N+1, -1);
		dp[0] = 0, prv[0] = -2;
		// inclusive range
		auto solve = [&](this auto&& self, int lo, int hi) -> void {
			assert(0 <= lo);
			assert(lo <= hi);
			int hi_orig = hi;
			hi = std::min(hi, lo + N-1);
			if (lo == 0) {
				hi = 0;
			} else {
				self((std::max(0, lo - N) + 1) / 2, (hi + N) / 2);
				for (int cur = lo; cur <= hi; cur++) {
					if (prv[cur] == -1) {
						assert(cur > N);
						for (int i = cur & 1; i <= N; i += 2) {
							int64_t cnd = dp[(cur+i)/2] + dp[(cur-i)/2];
							if (cnd > dp[cur]) {
								dp[cur] = cnd;
								prv[cur] = (cur+i)/2;
							}
						}
					}
				}
			}
			for (int cur = hi+1; cur <= hi_orig; cur++) {
				if (prv[cur] == -1) {
					assert(cur-N <= 0 || cur - N >= lo);
					for (int i = 1; i <= N && i <= cur; i++) {
						int64_t cnd = dp[cur-i] + A[i-1];
						if (cnd > dp[cur]) {
							dp[cur] = cnd;
							prv[cur] = i;
						}
					}
				}
			}
		};
		solve(0, N);
		if (num_best > N) {
			solve((N - 1) * best_idx + 1, N * best_idx);
			assert(dp[N * best_idx] < M);
			X[best_idx - 1] = num_best;
			ans = num_best * best_idx;
			for (int i = (N-1) * best_idx + 1; i <= N * best_idx; i++) {
				assert(prv[i] != -1);
				int64_t cost = dp[i];
				assert(cost < M);
				int64_t extra = (M - cost + A[best_idx-1] - 1) / A[best_idx];
				if (i + extra * best_idx < ans) {
					ans = i + extra * best_idx;
					X[best_idx - 1] = extra;
				}
			}
		} else {
			int hi = int(num_best) * best_idx;
			int lo = hi - best_idx + 1;
			solve(lo, hi);
			assert(dp[hi] >= M);
			ans = lo;
			while (dp[ans] < M) ans++;
		}
		[&](this auto&& self, int cur) -> void {
			assert(prv[cur] != -1);
			if (cur == 0) return;
			if (prv[cur] == cur) {
				X[cur-1]++;
			} else {
				self(prv[cur]);
				self(cur - prv[cur]);
			}
		}(int(ans - best_idx * X[best_idx-1]));

		for (int i = N-1; i > 0; i--) X[i-1] += X[i];
		cout << "Case #" << case_num << ": " << ans << '\n';
		for (int i = 0; i < N; i++) cout << X[i] << " \n"[i+1==N];
		cout << flush;
		assert(std::accumulate(X.begin(), X.end(), int64_t(0)) == ans);
	}

	return 0;
}
