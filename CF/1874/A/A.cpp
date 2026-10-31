#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		std::vector<int> A(N);
		std::vector<int> B(M);
		for (auto& a : A) cin >> a;
		for (auto& b : B) cin >> b;
		if (K >= 5) K -= (K - 5) / 2 * 2;
		for (int z = 0; z < K; z++) {
			if (z & 1) {
				auto mi = std::min_element(B.begin(), B.end());
				auto ma = std::max_element(A.begin(), A.end());
				if (*mi < *ma) std::swap(*mi, *ma);
			} else {
				auto mi = std::min_element(A.begin(), A.end());
				auto ma = std::max_element(B.begin(), B.end());
				if (*mi < *ma) std::swap(*mi, *ma);
			}
		}
		cout << std::accumulate(A.begin(), A.end(), int64_t(0)) << '\n';
	}

	return 0;
}
