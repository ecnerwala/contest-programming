#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A; A.reserve(N); A.resize(N-1);
		for (auto& a : A) cin >> a;

		auto solve = [&]() -> bool {
			int64_t tot = int64_t(N) * int64_t(N+1) / 2;
			if (A.back() > tot) return false;
			if (A.back() != tot) {
				A.push_back(tot);
			}
			for (int i = int(A.size()) - 1; i-1 >= 0; i--) {
				assert(A[i] > A[i-1]);
				A[i] -= A[i-1];
			}

			std::sort(A.begin(), A.end());

			if (int(A.size()) == N) {
				// good iff it's exactly 1..N
				for (int i = 0; i < N; i++) {
					if (A[i] != i+1) return false;
				}
				return true;
			} else {
				assert(int(A.size()) == N-1);
				int cnt = 0;
				for (int i = 0, j = 1; i < int(A.size()) && j <= N; ) {
					// need all but 2
					if (A[i] == j) {
						i++, j++;
						cnt++;
					} else if (A[i] > j) {
						j++;
					} else if (A[i] < j) {
						i++;
					} else assert(false);
				}
				assert(cnt != N-1);
				return cnt == N-2;
			}
		};
		cout << (solve() ? "YES" : "NO") << '\n';
	}

	return 0;
}
