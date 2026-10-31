#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> B(N); for (auto& b : B) std::cin >> b;
		std::vector<int> A; A.reserve(2*N);
		A.push_back(B[0]);
		for (int i = 1; i < N; i++) {
			if (B[i] < A.back()) A.push_back(B[i]);
			A.push_back(B[i]);
		}

		std::cout << A.size() << '\n';
		for (int i = 0; i < int(A.size()); i++) {
			std::cout << A[i] << " \n"[i+1==int(A.size())];
		}
	}

	return 0;
}
