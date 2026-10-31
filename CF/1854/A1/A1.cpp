#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// make them all positive or all negative
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::vector<std::pair<int, int>> ops; ops.reserve(50);
		auto do_op = [&](int i, int j) -> void {
			A[i] += A[j];
			ops.push_back({i, j});
		};
		int argmin = int(std::min_element(A.begin(), A.end()) - A.begin());
		int argmax = int(std::max_element(A.begin(), A.end()) - A.begin());
		if (A[argmax] + A[argmin] >= 0) {
			for (int i = 0; i < N; i++) {
				do_op(i, argmax);
			}
			for (int i = 1; i < N; i++) {
				do_op(i, i-1);
			}
		} else {
			for (int i = 0; i < N; i++) {
				do_op(i, argmin);
			}
			for (int i = N-1; i >= 1; i--) {
				do_op(i-1, i);
			}
		}

		cout << ops.size() << '\n';
		for (auto [i, j] : ops) {
			cout << i+1 << ' ' << j+1 << '\n';
		}
	}

	return 0;
}
