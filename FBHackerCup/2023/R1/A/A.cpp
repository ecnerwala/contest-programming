#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<int> X(N);
		for (auto& x : X) cin >> x;
		std::sort(X.begin(), X.end());

		assert(N >= 4);
		int double_ans;
		if (N == 4) {
			double_ans = (X[3] + X[2]) - (X[0] + X[1]);
		} else if (N == 5) {
			double_ans = std::max((X[4] + X[2]) - (X[0] + X[1]), (X[4] + X[3]) - (X[0] + X[2]));
		} else if (N >= 6) {
			double_ans = (X[N-1] + X[N-2]) - (X[0] + X[1]);
		} else assert(false);

		assert(double_ans >= 0);
		cout << "Case #" << case_num << ": " << double_ans / 2 << (double_ans & 1 ? ".5" : "") << '\n';
	}

	return 0;
}
