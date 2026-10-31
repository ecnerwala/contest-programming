#include<bits/stdc++.h>

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M; cin >> N >> M;
	struct edge_t {
		array<int, 2> dest;
		array<int, 2> cost;
		int idx;
		bool is_used;
	};
	vector<edge_t> edges(M);
	for (auto& e : edges) {
		cin >> e.dest[0] >> e.dest[1];
		cin >> e.cost[0] >> e.cost[1];
	}

	vector<int> par(N);
	auto run_kruskal = [&]() -> array<int, 2> {
		fill(par.begin(), par.end(), -1);
		auto get_par = std::y_combinator([&](auto self, int a) -> int {
			if (par[a] < 0) return a;
			return par[a] = self(par[a]);
		});
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[b] < par[a]) swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};
		array<int, 2> tot_cost{0,0};
		for (auto& e : edges) {
			if ((e.is_used = merge(e.dest[0], e.dest[1]))) {
				tot_cost[0] += e.cost[0];
				tot_cost[1] += e.cost[1];
			}
		}
		return tot_cost;
	};

	auto get_hull = [&](array<int, 2> coeff) -> array<int, 2> {
		assert(coeff[0] >= 0 && coeff[1] >= 0);
		sort(edges.begin(), edges.end(), [&](const edge_t& a, const edge_t& b) {
			return make_tuple(a.cost[0] * coeff[0] + a.cost[1] * coeff[1], a.cost) < make_tuple(b.cost[0] * coeff[0] + b.cost[1] * coeff[1], b.cost);
		});
		return run_kruskal();
	};

	array<int, 2> c0{1, 0};
	array<int, 2> c1{0, 1};
	array<int, 2> val_0 = get_hull(c0);
	array<int, 2> val_1 = get_hull(c1);
	pair<int64_t, array<int, 2>> best(int64_t(val_0[0]) * int64_t(val_0[1]), c0);
	best = min(best, {int64_t(val_1[0]) * int64_t(val_1[1]), c1});

	std::y_combinator([&](auto self, array<int, 2> v0, array<int, 2> v1) -> void {
		assert(v0[0] <= v1[0] && v0[1] >= v1[1]);
		if (v0 == v1) return;
		array<int, 2> c{v0[1] - v1[1], v1[0] - v0[0]};
		array<int, 2> vm = get_hull(c);
		if (vm == v0) return;
		best = min(best, {int64_t(vm[0]) * int64_t(vm[1]), c});
		self(v0, vm);
		self(vm, v1);
	})(val_0, val_1);

	auto tot_cost = get_hull(best.second);
	cout << tot_cost[0] << ' ' << tot_cost[1] << '\n';
	for (const auto& e : edges) {
		if (e.is_used) {
			cout << e.dest[0] << ' ' << e.dest[1] << '\n';
		}
	}

	return 0;
}
