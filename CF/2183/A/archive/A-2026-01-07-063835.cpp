#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		// if it's all 1's, alice wins
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		assert(N >= 3);
		if (1 == *std::min(A.begin(), A.end())) {
			cout << "Alice" << '\n';
		} else if (A[0] == 0 && A[N-1] == 0) {
			cout << "Bob" << '\n';
		} else {
			cout << "Alice" << '\n';
		}
	}

	return 0;
}
