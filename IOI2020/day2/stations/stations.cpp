#include "stations.h"
#include <vector>
#include <cassert>
#include <cmath>
#include <algorithm>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

std::vector<int> label(int N, int K, std::vector<int> U, std::vector<int> V) {
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		adj[U[e]].push_back(V[e]);
		adj[V[e]].push_back(U[e]);
	}

	std::vector<int> labels(N);

	int cur_idx = 0;
	std::y_combinator([&](auto self, int cur, int prv, bool dir) -> void {
		if (!dir) {
			labels[cur] = cur_idx++;
		}
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur, !dir);
		}
		if (dir) {
			labels[cur] = cur_idx++;
		}
	})(0, -1, false);

	assert(cur_idx-1 <= K);

	return labels;
}

int find_next_station(int S, int T, std::vector<int> C) {
	assert(S != T);
	assert(!C.empty());
	if (C.front() < S) {
		if (T > S || T <= C.front()) return C.front();
		auto it = --std::upper_bound(C.begin(), C.end(), T);
		return *it;
	} else {
		if (T < S || T >= C.back()) return C.back();
		auto it = std::lower_bound(C.begin(), C.end(), T);
		return *it;
	}
}
