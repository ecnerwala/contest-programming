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
		std::vector<int> A_freq; A_freq.reserve(N);
		A_freq.push_back(0);
		for (int i = 0, j = 0; i < N; i = j) {
			while (j < N && A[j] == A[i]) j++;
			A_freq.push_back(A_freq.back() + j);
		}
		cerr << A_freq.back() << ' ' << N << '\n';

		std::sort(B.begin(), B.end());
		int64_t pref_B = 0;
		for (int k = M; k >= 0; k--) {
		}

		cout << "Case #" << case_num << ": ";
	}

	return 0;
}
