#include "fish.h"

#include <vector>
#include <algorithm>

long long max_weights(int N, int M, std::vector<int> X, std::vector<int> Y, std::vector<int> W) {
	std::vector<std::vector<std::pair<int, int>>> fish(N);
	for (int e = 0; e < M; e++) {
		fish[X[e]].push_back({Y[e], W[e]});
	}
	for (int x = 0; x < N; x++) {
		std::sort(fish[x].begin(), fish[x].end());
	}

	auto normalize = [&](std::vector<std::pair<int, int64_t>>& v) {
		int l = 0;
		for (int i = 0; i < int(v.size()); i++) {
			if (l == 0 || v[i].second != v[l-1].second) {
				v[l++] = v[i];
			}
		}
		v.resize(l);
	};

	std::vector<std::pair<int, int64_t>> dp_decr;
	dp_decr.push_back({0, 0});
	std::vector<std::pair<int, int64_t>> dp_incr;
	dp_incr.push_back({0, 0});
	std::vector<std::pair<int, int64_t>> dp_zero;
	dp_zero.push_back({0, 0});

	for (int x = 0; x < N; x++) {
		std::vector<std::pair<int, int64_t>> ndp_decr;
		std::vector<std::pair<int, int64_t>> ndp_incr;
		std::vector<std::pair<int, int64_t>> ndp_zero;

		normalize(ndp_decr);
		normalize(ndp_incr);
		normalize(ndp_zero);
		dp_decr = std::move(ndp_decr);
		dp_incr = std::move(ndp_incr);
		dp_zero = std::move(ndp_zero);
	}
	return 0;
}
