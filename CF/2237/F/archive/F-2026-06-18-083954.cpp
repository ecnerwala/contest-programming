#include <bits/stdc++.h>
#include <cassert>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		std::vector<int> A(N);
		for (auto& a : A) { std::cin >> a; a--; }
		// number of valid starts
		const int INF = 2 * N + 2;
		std::vector<int> dp_1(N+1, -INF);
		std::vector<int> dp_M(N+1, -INF);
		std::vector<int> dp_use(N+1, -INF);

		std::vector<int> prv(N-M+1, -1);

		dp_1[0] = 0;
		dp_M[M] = 0;

		for (int i = 0; i < N; i++) {
			setmax(dp_1[i+1], dp_1[i]);
			setmax(dp_M[i+1], dp_M[i]);
			int st = i - A[i];
			if (0 <= st && st <= N - M) {
				setmax(dp_use[i], dp_1[st] + 1);
				setmax(dp_use[i], dp_M[i] + 1);
				if (prv[st] != -1) setmax(dp_use[i], dp_use[prv[st]] + 1);
				prv[st] = i;
				setmax(dp_M[st + M], dp_use[i]);
				setmax(dp_1[i+1], dp_use[i]);
			}
		}
		std::cout << N - dp_M[N] << '\n';
	}

	return 0;
}
