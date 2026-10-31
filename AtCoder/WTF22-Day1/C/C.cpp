#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	// b is +, a is -
	struct state_t {
		int max_alternating;
		int min_alternating;
		int extra;
		bool a_start;
		bool b_start;
	};
	auto merge = [&](state_t x, state_t y) -> state_t {
		state_t r;
		r.extra = x.extra + y.extra;
		r.a_start = x.a_start || y.a_start;
		r.b_start = x.b_start || y.b_start;
		r.max_alternating = x.max_alternating + y.max_alternating +
			(std::abs(x.extra) + std::abs(y.extra) - std::abs(x.extra + y.extra)) / 2;
		if (y.max_alternating < x.min_alternating) {
			std::swap(x, y);
		}
		if (x.max_alternating < y.min_alternating) {
			r.min_alternating = y.min_alternating - x.max_alternating;
			if (x.b_start && x.extra > 0) {
			}
			if (x.a_start && x.extra < 0) {
			}
		} else {
			r.min_alternating = 0;
		}
	};
	struct dp_t {
	};
	for (int i = 0; i < N; i++) {
	}

	return 0;
}
