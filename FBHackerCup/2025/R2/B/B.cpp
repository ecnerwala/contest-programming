#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, M; cin >> N >> M;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		std::vector<int> B(M); for (auto& b : B) cin >> b;

		std::sort(A.begin(), A.end(), std::greater<>());
		std::vector<int64_t> A_freq; A_freq.reserve(N);
		A_freq.push_back(0);
		for (int i = 0, j = 0; i < N; i = j) {
			while (j < N && A[j] == A[i]) j++;
			A_freq.push_back(A_freq.back() + j);
		}

		std::sort(B.begin(), B.end());
		int ans = M;
		int64_t pref_B = 0;
		for (int k = M-1, j = 0; k >= 0; k--) {
			pref_B += B[M-1-k];
			while (j+1 < int(A_freq.size()) && pref_B >= A_freq[j+1]) j++;
			ans = std::min(ans, (j ? int(A_freq[j] + A_freq[j-1]) : 0) + k);
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
