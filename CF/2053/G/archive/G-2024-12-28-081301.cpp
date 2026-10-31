#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {
		int N, M; cin >> N >> M;
		std::string S; cin >> S;
		std::string T; cin >> T;
		std::array<std::vector<int>, 2> max_match;
		for (int z = 0; z < 2; z++) {
			std::vector<int> z_fn(N, -1);
			z_fn[0] = N;
			for (int best_i = 1, best_j = 1, i = 1; i < N; i++) {
				int j = std::min(best_j, i + z_fn[i - best_i]);
				j = std::max(j, i);
				if (j < best_j) {
					assert(S[j] != S[j-i]);
				}

				while (j < N && S[j] == S[j-i]) {
					j++;
				}
				if (j >= best_j) {
					best_i = i;
					best_j = j;
				}
				z_fn[i] = j - i;
			}
			max_match[z].resize(M);
			for (int best_i = 0, best_j = 0, i = 0; i < M; i++) {
				int j = std::min(best_j, i + (i - best_i >= N ? 0 : z_fn[i - best_i]));
				j = std::max(j, i);
				while (j < M && j-i < N && T[j] == S[j-i]) {
					j++;
				}
				if (j >= best_j) {
					best_i = i;
					best_j = j;
				}
				max_match[z][i] = j - i;
			}
			std::reverse(S.begin(), S.end());
			std::reverse(T.begin(), T.end());
		}
		std::reverse(max_match[1].begin(), max_match[1].end());
	}

	return 0;
}
