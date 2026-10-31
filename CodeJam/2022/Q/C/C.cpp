#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		std::sort(A.begin(), A.end());
		int cnt = 0;
		for (int i = 0; i < N; i++) {
			if (A[i] >= cnt+1) cnt++;
		}

		cout << "Case #" << case_num << ": " << cnt << '\n';
	}

	return 0;
}
