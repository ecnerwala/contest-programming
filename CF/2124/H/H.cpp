#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		N++;
		std::vector<int> A(N);
		A[0] = 0;
		for (int i = 1; i < N; i++) cin >> A[i];

		std::vector<int> max_len_(N * (N+1) / 2);
		auto max_len = [&](int i, int j) -> int& {
			assert(0 <= i && i <= j && j < N);
			return max_len_[j * (j+1) / 2 + i];
		};

		// from each point, what's the max you can grow jumping to another of the equal
		for (int i = N-1; i >= 0; i--) {
			int Ai = A[i];
			std::vector<int> first_vis(N+1, -1);
			int cur_best = Ai-1;
			for (int j = i; j < N; j++) {
				int Aj = A[j];
				if (Aj == Ai) {
					max_len(i, j) = ++cur_best;
				} else if (Aj < Ai) {
					// do nothing
				} else if (Aj <= cur_best + 1) {
					if (first_vis[Aj] == -1) first_vis[Aj] = j;
					cur_best = std::max(cur_best, max_len(first_vis[A[j]], j));
				}
			}
			if (i == 0) cout << cur_best << '\n';
		}
	}

	return 0;
}
