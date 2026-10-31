#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K; K--;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		int V = A[K];
		int best = 0;
		auto check = [&]() -> void {
			int cnd = 0;
			int cur = A[0];
			for (int i = 1; i < N; i++) {
				cur = std::max(cur, A[i]);
				if (cur == V) cnd++;
			}
			best = std::max(best, cnd);
		};

		std::swap(A[K], A[0]);
		check();
		std::swap(A[K], A[0]);

		std::swap(A[K], A[1]);
		check();
		std::swap(A[K], A[1]);

		for (int i = 0; i < N; i++) {
			if (A[i] >= V) {
				std::swap(A[K], A[i]);
				check();
				std::swap(A[K], A[i]);
				break;
			}
		}
		cout << best << '\n';
	}

	return 0;
}
