#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		vector<int64_t> A(N); for (auto& a : A) cin >> a;
		vector<int64_t> D(N-1);
		for (int i = 0; i+1 < N; i++) {
			D[i] = A[i+1] - A[i];
		}

		int ans = 1;
		int cur = 1;
		for (int i = 1; i < N-1; i++) {
			if (D[i] == D[i-1]) {
				cur++;
			} else {
				cur = 1;
			}
			ans = max(ans, cur);
		}

		cout << "Case #" << case_num << ": " << ans+1 << '\n';
	}

	return 0;
}


// max arith subarray = 1 + max equal subarray in the differenes
