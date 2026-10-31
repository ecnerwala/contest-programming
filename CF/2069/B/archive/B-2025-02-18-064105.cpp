#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::vector<int>> A(N, std::vector<int>(M));
		std::vector<int> cost(N*M, 0);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				int a; cin >> a; a--;
				A[i][j] = a;
				if (cost[a] == 0) cost[a] = 1;
				if (i > 0 && A[i-1][j] == a) cost[a] = 2;
				if (j > 0 && A[i][j-1] == a) cost[a] = 2;
			}
		}
		int ans = std::accumulate(cost.begin(), cost.end(), 0);
		int mx = *std::max_element(cost.begin(), cost.end());
		cout << ans - mx << '\n';
	}

	return 0;
}
