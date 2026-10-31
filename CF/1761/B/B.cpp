#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		bool is_alternating = true;
		for (int i = 0; i < N; i++) {
			if (A[i] != A[(i+2)%N]) is_alternating = false;
		}
		if (N == 1) {
			cout << 1 << '\n';
		} else if (is_alternating) {
			cout << (N+2)/2 << '\n';
		} else {
			cout << N << '\n';
		}
	}

	return 0;
}
