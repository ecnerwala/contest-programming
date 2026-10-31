#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) { cin >> a; }
		std::swap(*A.begin(), *std::min_element(A.begin(), A.end()));
		int64_t g = 0;
		for (int i = 1; i < N; i++) {
			if (A[i] % A[0] == 0) g = std::gcd(A[i], g);
		}
		if (g == A[0]) {
			cout << "Yes" << '\n';
		} else {
			cout << "No" << '\n';
		}
	}

	return 0;
}
