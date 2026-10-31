#include "nile.h"

#include <bits/stdc++.h>

std::vector<long long> calculate_costs(
	std::vector<int> W,
	std::vector<int> A, std::vector<int> B,
	std::vector<int> E
) {
	int N = int(W.size());
	assert(N == int(A.size()));
	assert(N == int(B.size()));
	int Q = int(E.size());

	struct artifact_t {
		int w, i, delta;
	};
	std::vector<artifact_t> artifacts(N);
	for (int i = 0; i < N; i++) {
		artifacts[i] = { W[i], i, A[i] - B[i] };
	}
	std::sort(artifacts.begin(), artifacts.end(), [&](const artifact_t& a, const artifact_t& b) {
		return a.w < b.w;
	});

	struct evt_t {
		int v;
		int t;
		int idx;
	};
	std::vector<evt_t> events; events.reserve(2 * N - 1 + Q);
	for (int d = 1; d <= 2; d++) {
		for (int i = 0; i+d < N; i++) {
			events.push_back({
				artifacts[i+d].w - artifacts[i].w,
				d,
				i
			});
		}
	}
	for (int q = 0; q < Q; q++) {
		events.push_back({E[q], 3, q});
	}
	std::sort(events.begin(), events.end(), [&](const evt_t& a, const evt_t& b) {
		return std::tie(a.v, a.t) < std::tie(b.v, b.t);
	});

	std::vector<long long> ans(Q);

	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) {
				par[a] = par[par[a]];
			}
			a = par[a];
		}
		return a;
	};

	struct state_t {
		int balance;
		std::array<int, 2> cost;
	};
	const int INF = int(1e9) + 10;
	std::vector<state_t> states(N);
	int64_t tot_cost = std::accumulate(A.begin(), A.end(), int64_t(0));
	auto get_state_cost = [&](const state_t& s) -> int {
		if (s.balance == 1) return s.cost[0];
		else if (s.balance == -1) return s.cost[1];
		else return 0;
	};
	for (int i = 0; i < N; i++) {
		states[i].balance = (i & 1) ? -1 : +1;
		states[i].cost[i & 1] = artifacts[i].delta;
		states[i].cost[!(i & 1)] = INF;
	}
	
	auto setmin = [](auto& a, auto b) { if (b < a) a = b; };

	auto merge = [&](int a, int b) -> void {
		a = get_par(a);
		b = get_par(b);
		assert(a != b);
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		tot_cost -= get_state_cost(states[a]) + get_state_cost(states[b]);
		states[a].balance += states[b].balance;
		setmin(states[a].cost[0], states[b].cost[0]);
		setmin(states[a].cost[1], states[b].cost[1]);
		tot_cost += get_state_cost(states[a]);
	};

	for (auto [_, t, idx] : events) {
		if (t == 1) {
			merge(idx, idx + 1);
		} else if (t == 2) {
			int a = get_par(idx);
			assert(a == get_par(idx + 1));
			assert(a == get_par(idx + 2));
			tot_cost -= get_state_cost(states[a]);
			setmin(states[a].cost[idx & 1], artifacts[idx + 1].delta);
			tot_cost += get_state_cost(states[a]);
		} else if (t == 3) {
			ans[idx] = tot_cost;
		} else assert(false);
	}

	return ans;
}
