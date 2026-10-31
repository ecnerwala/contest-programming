#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int> B(M);
		for (auto& b : B) cin >> b;

		int all_b = 0;
		for (auto b : B) all_b |= b;

		int x = 0, y = 0;
		for (auto a : A) {
			x ^= a;
			y ^= a | all_b;
		}
		cout << std::min(x, y) << ' ' << std::max(x, y) << '\n';
	}

	return 0;
}
