#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M, L; std::cin >> N >> M >> L;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		// M*L is small
		int idx = 0;
		M = std::min(M, N - idx + 1);
		std::vector<int> vals(M);
		for (int t = 1; t <= L; t++) {
			{
				// take the first min, and increment it
				++*std::min_element(vals.begin(), vals.end());
			}
			if (idx < N && t == A[idx]) {
				// shine a flashlight
				idx++;
				vals.erase(vals.begin());
				M = std::min(M, N - idx + 1);
				vals.resize(M, 0);
			}
		}
		assert(idx == N);
	}

	return 0;
}
