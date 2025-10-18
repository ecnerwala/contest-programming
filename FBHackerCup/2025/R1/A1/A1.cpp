#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		int ans = 0;
		for (int i = 0; i+1 < N; i++) {
			ans = std::max(ans, std::abs(A[i] - A[i+1]));
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
