#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) {
			cin >> a;
		}
		while (int(A.size()) >= 2) {
			std::nth_element(A.begin(), A.begin() + 1, A.end());
			A[0] += A[1];
			A[0] /= 2;
			A.erase(A.begin() + 1);
		}
		cout << A[0] << '\n';
	}

	return 0;
}
