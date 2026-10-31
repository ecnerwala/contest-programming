#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		int64_t D; cin >> D;
		vector<int64_t> X(N);
		for (int i = 0; i < N; i++) cin >> X[i];
		for (int i = N-1; i >= 0; i--) {
			D = D / X[i] * X[i];
		}

		cout << "Case #" << case_num << ": " << D << '\n';
	}

	return 0;
}
