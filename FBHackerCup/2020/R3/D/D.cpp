#include<bits/stdc++.h>
#include <numeric>

// Intuition: if you have like >= 2 quarters, you're probably fine

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		vector<int> A(N), B(N);
		for (int i = 0; i < N; i++) {
			cin >> A[i] >> B[i]; A[i]--, B[i]--;
		}

		cerr << "Case #" << case_num << ": " << "N = " << N << '\n';

		vector<int> A_dist(N+2);
		vector<int> A_chain(N+2);
		int init_C = 0;
		for (int i = 0; i < N+2; i++) {
			int l = 0;
			int cur;
			for (cur = i; cur < N; cur = A[cur]) l++;
			A_dist[i] = l;
			A_chain[i] = cur;
			init_C = max(init_C, l);
		}
		// We start with two chains of length init_C and N - init_C
		assert(2 * init_C >= N);

		vector<int> B_dist(N+2);
		vector<int> B_chain(N+2);
		int final_C = 0;
		for (int i = 0; i < N+2; i++) {
			int l = 0;
			int cur;
			for (cur = i; cur < N; cur = B[cur]) l++;
			B_dist[i] = l;
			B_chain[i] = cur;
			final_C = max(final_C, l);
		}
		vector<int> inv_B(N+2, -1);
		for (int i = 0; i < N; i++) inv_B[B[i]] = i;

		vector<int64_t> ans(N+1);

		{ // Q = 0
			int v = 0;
			for (int i = 0; i < N; i++) v += A[i] == B[i];
			//cerr << "F(0, *, *, *) = " << v << " x " << int64_t(N - init_C + 1) * int64_t(N - init_C + 1) * int64_t(N - (N - init_C) + 1) << '\n';
			ans[v] += int64_t(N - init_C + 1) * int64_t(N - init_C + 1) * int64_t(N - (N - init_C) + 1);
		}

		{ // Q = 1
			for (int c = init_C; c <= N; c++) {
				int num_good = 0;
				for (int i = 0; i < N; i++) {
					if (A_dist[i] <= N-c) {
						// we're stuck
						num_good += A[i] == B[i];
					} else {
						if (A_dist[B[i]] < N-c) {
							// sucks, we're buried
						} else {
							num_good++;
						}
					}
				}

				array<int, 2> base_len{-1, -1};
				vector<int> extras; extras.reserve(N);

				for (int i = 0; i < N+2; i++) {
					if (A_dist[i] < N-c) continue;
					if (inv_B[i] == -1 || A_dist[inv_B[i]] <= N-c) {
						// we're a tail
						int cur;
						int l = 0;
						for (cur = i; A_dist[cur] > N-c; cur = B[cur]) l++;
						assert(A_dist[cur] <= N-c);
						if (A_dist[cur] == N-c) {
							l += N-c;
							base_len[A_chain[cur] - N] = l;
						} else {
							extras.push_back(l);
						}
					}
				}
				assert(base_len[0] != -1);
				assert(base_len[1] != -1);
				assert(std::accumulate(extras.begin(), extras.end(), base_len[0]+base_len[1]) == N);

				vector<bool> poss(N+1);
				poss[base_len[0]] = true;
				for (int v : extras) {
					for (int i = N; i >= v; i--) {
						if (poss[i-v]) poss[i] = true;
					}
				}

				// num_good is the optimal case
				for (int x = 0; x <= N; x++) {
					int l1 = min(c, x);
					int first_good = l1;
					while (first_good >= 0 && !poss[first_good]) first_good--;

					for (int y = 0; y <= N; y++) {
						int l2 = min(c, y);
						if (l1 + l2 < N) continue;

						int v = num_good - (first_good >= 0 && N-first_good <= l2 ? 0 : 1);
						//cerr << "F(" << 1 << ',' << c << ',' << x << ',' << y << ") = " << v << '\n';
						ans[v]++;
					}
				}
			}
		}

		{ // Q >= 2: no problems, you can get any 2 chains; only issue is final configuration
			for (int c = init_C; c <= N; c++) {
				for (int x = 0; x <= N; x++) {
					for (int y = 0; y <= N; y++) {
						int l1 = min(c, x);
						int l2 = min(c, y);
						if (l1 + l2 < N) continue;

						int num_cuts = (N - final_C > l1 || final_C > l2);
						// number of ways for Q
						//cerr << "F(" << "2+" << ',' << c << ',' << x << ',' << y << ") = " << N - num_cuts << " x " << int64_t(N-2+1) << '\n';
						ans[N - num_cuts] += int64_t(N - 2 + 1);
					}
				}
			}
		}

		int64_t impossible = int64_t(N+1) * int64_t(N+1) * int64_t(N+1) * int64_t(N+1);
		for (int i = 0; i <= N; i++) impossible -= ans[i];

		cerr << "Case #" << case_num << ": " << impossible << '\n';

		cout << "Case #" << case_num << ": " << impossible;
		for (int i = 0; i <= N; i++) {
			cout << ' ' << ans[i];
		}
		cout << '\n';
	}

	return 0;
}
