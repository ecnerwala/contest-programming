#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> A(N+M);
		for (auto& a : A) cin >> a;
		std::nth_element(A.begin(), A.end() - N, A.end()-1);
		std::cout << std::accumulate(A.end() - N, A.end(), int64_t(0)) << '\n';
	}

	return 0;
}
