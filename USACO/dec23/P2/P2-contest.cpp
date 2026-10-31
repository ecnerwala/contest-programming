#include <bits/stdc++.h>

template <int MOD> struct modnum {
	int v;
	modnum() : v(0) {}
	modnum(int64_t _v) : v((int(_v % MOD) + MOD) % MOD) {}
	explicit operator int() const { return v; }
	modnum& operator += (modnum o) { v -= MOD - o.v; v = (v < 0 ? v + MOD : v); return *this; }
	friend modnum operator + (modnum a, modnum b) { return modnum(a) += b; }
	friend modnum operator * (modnum a, modnum b) { return modnum(int64_t(a.v) * int64_t(b.v)); }
};

template <typename num> struct lin_fn {
	num m, x;
	// compose in the standard math order, a(b(x))
	friend lin_fn operator * (lin_fn a, lin_fn b) { return {a.m * b.m, a.m * b.x + a.x}; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<int(1e9)+7>;

	int N; cin >> N;
	int M; cin >> M;

	std::vector<int> par(2*N-1, -1);
	std::vector<lin_fn<num>> ops(2*N-1, {1, 0});
	auto get_par = [&](int a) -> std::pair<int, lin_fn<num>> {
		lin_fn<num> res{1, 0};
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) {
				ops[a] = ops[par[a]] * ops[a];
				par[a] = par[par[a]];
			}
			res = ops[a] * res;
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
		lin_fn<num> e_op{10, edge_id+1};
		ops[pa] = b_op * e_op;
		par[pa] = e;
		ops[pb] = a_op * e_op;
		par[pb] = e;
	}
	for (int i = 0; i < N; i++) {
		auto [pa, a_op] = get_par(i);
		cout << int(a_op.x) << '\n';
	}

	return 0;
}
