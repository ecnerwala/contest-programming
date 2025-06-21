#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int N = 13;
	int N_prob, M, Q; cin >> N_prob >> M >> Q;

	auto choose = [&](int n, int r) -> int {
		int a = 1;
		for (int i = 0; i < r; i++) a *= n-i, a /= i+1;
		return a;
	};

	std::vector<std::vector<int>> poss_weights(N+1);
	poss_weights[0].push_back(0);
	for (int c = 0; c <= N; c++) {
		std::sort(poss_weights[c].begin(), poss_weights[c].end());
		poss_weights[c].resize(std::unique(poss_weights[c].begin(), poss_weights[c].end()) - poss_weights[c].begin());
		for (int d = 1; c + d <= N; d++) {
			for (auto v : poss_weights[c]) {
				poss_weights[c+d].push_back(2 * v + d);
			}
		}
		cerr << c << ' ' << int(poss_weights[c].size()) << ' ' << choose(N, c) * poss_weights[c].size() * (1 << (N-c)) << '\n';
	}
	std::vector<std::vector<std::vector<int>>> trans(N+1);
	for (int c = 0; c <= N; c++) {
		trans[c].resize(N-c+1);
		for (int d = 1; c + d <= N; d++) {
			trans[c][d].resize(poss_weights[c].size());
			for (int z = 0; z < int(poss_weights[c].size()); z++) {
				int v = poss_weights[c][z];
				trans[c][d][z] = int(std::lower_bound(poss_weights[c+d].begin(), poss_weights[c+d].end(), 2 * v + d) - poss_weights[c+d].begin());
				assert(poss_weights[c+d][trans[c][d][z]] == 2 * poss_weights[c][z] + d);
			}
		}
	}

	std::array<std::array<int, N>, N> weight;
	for (int i = 0; i < N; i++) {
		weight[i].fill(0);
	}

	for (int e = 0; e < M; e++) {
		int a, b, t; cin >> a >> b >> t; a--, b--;
		weight[a][b] += t;
	}

	return 0;
}
