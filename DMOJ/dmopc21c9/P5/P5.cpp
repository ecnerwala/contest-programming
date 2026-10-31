#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		if (N == 4) {
			cout << 0 << ' ' << 0 << ' ' << 0 << ' ' << 0 << '\n';
			continue;
		}
		std::vector<int> ans(N);

		for (int i = 0; i < N; i++) {
			if (A[(i+N-1)%N] == A[(i+1)%N]) {
				ans[i] = A[(i+1)%N];
				if (A[(i+1)%N] < A[(i+3)%N]) {
					for (int j = (i+2)%N; A[(j+1)%N] < A[(j+3)%N]; j = (j+2)%N) {
						ans[j] = A[(j+1)%N];
					}
				}
				if (A[(i+N-1)%N] < A[(i+N-3)%N]) {
					for (int j = (i+N-2)%N; A[(j+N-1)%N] < A[(j+N-3)%N]; j = (j+N-2)%N) {
						ans[j] = A[(j+N-1)%N];
					}
				}
			}
		}

		std::vector<bool> used(N+1);
		std::vector<int> lower_bounds; lower_bounds.reserve(N);
		for (int i = 0; i < N; i++) {
			used[A[i]] = true;
			if (A[i] >= A[(i+N-2)%N] && A[i] >= A[(i+2)%N]) {
				lower_bounds.push_back(A[i]);
			}
		}
		std::sort(lower_bounds.begin(), lower_bounds.end());

		std::vector<int> unused; unused.reserve(lower_bounds.size());
		for (int v = 1; v <= N; v++) {
			if (!used[v]) unused.push_back(v);
		}
		assert(unused.size() == lower_bounds.size());

		for (int z = 0; z < int(unused.size()); z++) {
			assert(lower_bounds[z] < unused[z]);
		}

		for (int i = 0; i < N; i++) {
			if (A[i] >= A[(i+N-2)%N] && A[i] >= A[(i+2)%N]) {
				int z = int(std::lower_bound(lower_bounds.begin(), lower_bounds.end(), A[i]) - lower_bounds.begin());
				assert(lower_bounds[z] < unused[z]);
				if ((z == 0 || unused[z-1] < lower_bounds[z]) && (z+1 == int(unused.size()) || unused[z] < lower_bounds[z+1])) {
					if (A[i] == A[(i+N-2)%N]) {
						ans[(i+1)%N] = unused[z];
					} else if (A[i] == A[(i+2)%N]) {
						ans[(i+N-1)%N] = unused[z];
					}
				}
			}
		}

		for (int i = 0; i < N; i++) {
			cout << ans[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
