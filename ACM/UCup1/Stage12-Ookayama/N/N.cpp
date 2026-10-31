#include <bits/stdc++.h>

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

	int N, M, K; cin >> N >> M >> K;

	std::vector<std::pair<int, std::array<int, 2>>> edges(M);
	for (auto& [c, vs] : edges) {
		cin >> vs[0] >> vs[1] >> c;
		vs[0]--, vs[1]--;
	}

	std::sort(edges.begin(), edges.end(), [&](const auto& a, const auto& b) { return a.first < b.first; });

	int Q; cin >> Q;
	std::vector<std::pair<int, int>> queries(Q);
	for (int q = 0; q < Q; q++) {
		cin >> queries[q].first;
		queries[q].second = q;
	}

	std::sort(queries.begin(), queries.end(), [&](const auto& a, const auto& b) { return a.first < b.first; });

	std::vector<std::pair<int, int>> ops; ops.reserve(2*N);
	int64_t tot_pairs = 0;
	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) a = par[a];
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		ops.push_back({a, par[a]});
		ops.push_back({b, par[b]});
		tot_pairs += int64_t(par[a]) * int64_t(par[b]);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	std::vector<int64_t> ans(Q);

	auto apply_edges = [&](int elo, int ehi) -> std::pair<int64_t, int> {
		std::pair<int64_t, int> r = {tot_pairs, int(ops.size())};
		for (int e = elo; e < ehi; e++) {
			auto [u, v] = edges[e].second;
			merge(u, v);
		}
		return r;
	};

	auto reset = [&](std::pair<int64_t, int> r) {
		tot_pairs = r.first;
		while (int(ops.size()) > r.second) {
			auto [x, y] = ops.back();
			par[x] = y;
			ops.pop_back();
		}
	};

	std::y_combinator([&](auto self, int qlo, int qhi, int elo, int ehi, int layer) -> void {
		assert(qlo <= qhi);
		if (qlo == qhi) return;
		if (layer == -1) {
			for (int q = qlo; q < qhi; q++) {
				auto [_, idx] = queries[q];
				ans[idx] = tot_pairs;
			}
			return;
		}

		int qmi = qlo;
		while (qmi < qhi && !(queries[qmi].first & (1 << layer))) {
			qmi++;
		}
		int emi = elo;
		while (emi < ehi && !(edges[emi].first & (1 << layer))) {
			emi++;
		}

		if (K & (1 << layer)) {
			{
				auto r = apply_edges(elo, emi);
				self(qlo, qmi, emi, ehi, layer-1);
				reset(r);
			}

			{
				auto r = apply_edges(emi, ehi);
				self(qmi, qhi, elo, emi, layer-1);
				reset(r);
			}

		} else {
			self(qlo, qmi, elo, emi, layer-1);
			self(qmi, qhi, emi, ehi, layer-1);
		}
	})(0, Q, 0, M, 29);

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
