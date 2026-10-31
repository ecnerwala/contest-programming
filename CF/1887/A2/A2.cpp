#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto solve = [&](std::vector<int> A, const std::vector<int>& B) -> int {
		int N = int(A.size());
		std::sort(A.begin(), A.end());
		int j = 0;
		int ans = 0;
		for (int i = 0; i < N; i++) {
			while (j < N && A[i] >= B[j]) j++;
			ans = std::max(ans, j-i);
		}
		return ans;
	};

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int M; cin >> M;
		std::vector<int> A(N-1);
		for (auto& a : A) cin >> a;
		std::vector<int> B(N);
		for (auto& b : B) cin >> b;

		std::sort(A.begin(), A.end());
		std::sort(B.begin(), B.end());

		A.push_back(1);
		int v1 = solve(A, B);
		A.back() = M;
		int vM = solve(A, B);

		assert(vM - v1 == 1 || vM - v1 == 0);

		if (v1 < vM) {
			assert(vM - v1 == 1);
			int mi = 1, ma = M;
			while (ma - mi > 1) {
				int md = mi + (ma - mi) / 2;
				A.back() = md;
				int vmid = solve(A, B);
				if (vmid == v1) {
					mi = md;
				} else {
					ma = md;
				}
			}
			int64_t ans = int64_t(mi) * v1 + int64_t(M - mi) * vM;
			cout << ans << '\n';
		} else {
			cout << int64_t(M) * v1 << '\n';
		}

	}

	return 0;
}
