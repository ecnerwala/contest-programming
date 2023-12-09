#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int> pref_val(N+1);
		std::vector<int> suff_val(N+1);
		for (int i = 0; i < N; i++) {
			pref_val[i+1] = std::max(0, A[i] - pref_val[i]);
		}
		for (int i = N-1; i >= 0; i--) {
			suff_val[i] = std::max(0, A[i] - suff_val[i+1]);
		}

		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			ans += std::max(0, A[i] - pref_val[i] - suff_val[i+1]);
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
