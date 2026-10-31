#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int B, K; cin >> B >> K;
	int V = 1 << B;
	std::vector<int> A(V);
	for (auto& a : A) cin >> a;

	K = min(K, V/2);

	std::vector<int> atob(V, -1);
	std::vector<int> btoa(V, -1);

	std::vector<std::tuple<int, int, int>> a_best; a_best.reserve(V/2);
	for (int i = 0; i < V; i++) {
		if (__builtin_popcount(i) & 1) continue;
		std::tuple<int, int, int> cur{-1, i, -1};
		for (int z = 0; z < B; z++) {
			int j = i ^ (1 << z);
			cur = max(cur, {A[i] + A[j], i, j});
		}
		a_best.push_back(cur);
	}
	std::sort(a_best.begin(), a_best.end());
	std::reverse(a_best.begin(), a_best.end());
	auto a_best_it = a_best.begin();

	int64_t ans = 0;
	std::vector<int> cur_bs; cur_bs.reserve(K);
	for (int iter = 0; iter < K; iter++) {
		std::vector<tuple<int, int, int>> all_as; all_as.reserve(cur_bs.size() * B);
		for (int b : cur_bs) {
			assert(btoa[b] != -1);
			for (int z = 0; z < B; z++) {
				all_as.push_back({b ^ (1 << z), -1, -1});
			}
		}
		std::sort(all_as.begin(), all_as.end());
		all_as.resize(std::unique(all_as.begin(), all_as.end()) - all_as.begin());
		for (auto& [a, v, cb] : all_as) {
			for (int z = 0; z < B; z++) {
				int b = a ^ (1 << z);
				if (btoa[b] == -1) {
					if (A[b] > v) {
						v = A[b];
						cb = b;
					}
				}
			}
		}
		std::sort(all_as.begin(), all_as.end(), [&](auto x, auto y) { return std::get<1>(x) > std::get<1>(y); });
	}

	return 0;
}
