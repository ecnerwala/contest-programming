#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, X; cin >> N >> X;
		std::vector<std::pair<int, int>> A(N);
		for (auto& [a, _] : A) cin >> a;
		for (int i = 0; i < N; i++) A[i].second = i;
		std::vector<int> B(N);
		for (auto& b : B) cin >> b;

		std::sort(A.begin(), A.end());
		std::sort(B.begin(), B.end());
		std::rotate(B.begin(), B.begin() + X, B.end());

		std::vector<int> C(N);

		int beauty = 0;
		for (int i = 0; i < N; i++) {
			C[A[i].second] = B[i];
			beauty += (A[i].first > B[i]);
		}
		if (beauty == X) {
			cout << "YES" << '\n';
			for (int i = 0; i < N; i++) {
				cout << C[i] << " \n"[i+1==N];
			}
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
