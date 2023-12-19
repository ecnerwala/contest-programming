#include <bits/stdc++.h>

const int MOD = int(1e9)+7;
struct lin_fn {
	int m, x;
	// compose in the left to right application order
	friend lin_fn operator * (lin_fn a, lin_fn b) {
		return {int(int64_t(a.m) * int64_t(b.m) % MOD), int((int64_t(a.x) * int64_t(b.m) + b.x) % MOD)};
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int M; cin >> M;

	std::vector<int> par(2*N-1, -1);
	std::vector<lin_fn> ops(2*N-1, {1, 0});
	auto get_par = [&](int a) -> std::pair<int, lin_fn> {
		lin_fn res{1, 0};
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) {
				ops[a] = ops[a] * ops[par[a]];
				par[a] = par[par[a]];
			}
			res = res * ops[a];
			a = par[a];
		}
		return {a, res};
	};

	int V = N;
	for (int edge_id = 0; edge_id < M; edge_id++) {
		int a, b; cin >> a >> b; a--, b--;
		auto [pa, a_op] = get_par(a);
		auto [pb, b_op] = get_par(b);
		if (pa == pb) continue;
		int e = V++;
		lin_fn e_op{10, edge_id+1};
		ops[pa] = e_op * b_op;
		par[pa] = e;
		ops[pb] = e_op * a_op;
		par[pb] = e;
	}
	for (int i = 0; i < N; i++) {
		auto [pa, a_op] = get_par(i);
		cout << a_op.x << '\n';
	}

	return 0;
}
