#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		std::vector<int> A(2*N);
		for (auto& a : A) cin >> a;

		auto solve = [&]() -> int {
			int cnd = -1;
			if (N == 1) {
				cnd = 0;
			} else if (N & 1) {
				for (int i = 0; i < 2*N; i++) {
					if (A[i] == A[(i+N)%(2*N)] && A[(i+1)%(2*N)] > A[(i+1+N)%(2*N)]) {
						cnd = (i - (N-1)/2 + 2*N) % (2*N);
					}
				}
			} else {
				for (int i = 0; i < 2*N; i++) {
					if (A[i] < A[(i+N)%(2*N)] && A[(i+1)%(2*N)] > A[(i+1+N)%(2*N)]) {
						// (i+1)%2*N must be the end
						cnd = (i+1 - N/2 + 2*N)%(2*N);
					}
				}
			}
			//cerr << "cnd = " << cnd << '\n';
			if (cnd == -1) return -1;
			std::rotate(A.begin(), A.begin()+cnd, A.end());
			bool is_good = true;
			for (int i = 0; i < N-1-i; i++) {
				is_good = is_good && (A[i] < A[i+N]);
				is_good = is_good && (A[N-1-i] > A[N-1-i+N]);
			}
			/*
			for (int i = (N+1)/2; i < N; i++) {
				is_good = is_good && (A[i] > A[i+N]);
			}
			*/
			for (int i = 0; i < N; i++) {
				is_good = is_good && (A[i] == A[2*N-1-i]);
			}
			return is_good ? cnd : -1;
		};

		int ans = solve();
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
