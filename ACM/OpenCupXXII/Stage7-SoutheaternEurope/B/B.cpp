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

	int K, N, Q; cin >> K >> N >> Q;

	// Ops are to zero some subset and then add something
	struct range_t {
		std::array<int, 16> mi{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	};
	auto merge = [&](range_t a, range_t b) -> range_t {
		range_t r;
		for (int i = 0; i < 16; i++) {
			r.mi[i] = std::min(a.mi[i], b.mi[i]);
		}
		return r;
	};

	struct op_t {
		int zero = 0;
		std::array<int, 4> add = {0,0,0,0};
	};
	auto apply = [&](op_t o, range_t a) -> range_t {
		range_t r;
		r.mi[0] = 0;
		for (int i = 0; i < 4; i++) {
			for (int m = 0; m < (1 << i); m++) {
				r.mi[(1<<i)+m] = r.mi[m] + o.add[i];
			}
		}
		for (int i = 0; i < 16; i++) {
			r.mi[i] += a.mi[i & ~o.zero];
		}
		return r;
	};

	// apply a to b
	auto compose = [&](op_t a, op_t b) -> op_t {
		op_t r = b;
		for (int i = 0; i < 4; i++) {
			if (a.zero & (1 << i)) {
				r.add[i] = 0;
			}
			r.add[i] += a.add[i];
		}
		r.zero |= a.zero;
		return r;
	};

	struct node_t {
		int l, r;
		std::array<int, 2> c;
		range_t val;
		op_t lazy;
	};

	std::vector<node_t> nodes; nodes.reserve(N + Q * 40);

	std::vector<std::array<int, 4>> A_init(N, {0,0,0,0});
	for (int k = 0; k < K; k++) {
		for (int i = 0; i < N; i++) {
			cin >> A_init[i][k];
		}
	}

	std::vector<int> roots(Q+1, -1);
	roots[0] = std::y_combinator([&](auto self, int l, int r) -> int {
		assert(r > l);
		if (r - l == 1) {
			int a = int(nodes.size());
			nodes.emplace_back();
			nodes[a].l = l;
			nodes[a].r = r;
			nodes[a].c = {-1, -1};
			nodes[a].val.mi[0] = 0;
			for (int i = 0; i < 4; i++) {
				for (int m = 0; m < (1 << i); m++) {
					nodes[a].val.mi[(1<<i)+m] = nodes[a].val.mi[m] + A_init[l][i];
				}
			}
			nodes[a].lazy = op_t{};
			return a;
		}
		int m = (l + r) / 2;
		int a = int(nodes.size());
		nodes.emplace_back();
		int x = self(l, m);
		int y = self(m, r);
		nodes[a].l = l;
		nodes[a].r = r;
		nodes[a].c = {x, y};
		nodes[a].val = merge(nodes[x].val, nodes[y].val);
		nodes[a].lazy = op_t{};
		return a;
	})(0, N);

	auto do_query = std::y_combinator([&](auto self, int a, int l, int r, op_t cur_op, int qlo, int qhi) -> int {
		assert(a != -1 && l == nodes[a].l && r == nodes[a].r);
		assert(l < r && qlo < qhi);
		assert(qlo < r && l < qhi);

		if (qlo <= l && r <= qhi) {
			return apply(cur_op, nodes[a].val).mi[15];
		}
		cur_op = compose(cur_op, nodes[a].lazy);
		int m = (l + r) / 2;
		if (qhi <= m) {
			return self(nodes[a].c[0], l, m, cur_op, qlo, qhi);
		}
		if (m <= qlo) {
			return self(nodes[a].c[1], m, r, cur_op, qlo, qhi);
		}
		return std::min(self(nodes[a].c[0], l, m, cur_op, qlo, qhi), self(nodes[a].c[1], m, r, cur_op, qlo, qhi));
	});

	auto do_update = std::y_combinator([&](auto self, int a, int l, int r, op_t lazy_op, op_t new_op, int qlo, int qhi) -> int {
		assert(a != -1 && l == nodes[a].l && r == nodes[a].r);
		assert(l < r && qlo < qhi);

		nodes.push_back(nodes[a]);
		a = int(nodes.size())-1;

		if (r <= qlo || qhi <= l) {
			nodes[a].val = apply(lazy_op, nodes[a].val);
			nodes[a].lazy = compose(lazy_op, nodes[a].lazy);

			return a;
		}

		if (qlo <= l && r <= qhi) {
			new_op = compose(new_op, lazy_op);

			nodes[a].val = apply(new_op, nodes[a].val);
			nodes[a].lazy = compose(new_op, nodes[a].lazy);

			return a;
		}

		nodes[a].val = apply(lazy_op, nodes[a].val);
		nodes[a].lazy = compose(lazy_op, nodes[a].lazy);

		int m = (l + r) / 2;
		int x = self(nodes[a].c[0], l, m, nodes[a].lazy, new_op, qlo, qhi);
		int y = self(nodes[a].c[1], m, r, nodes[a].lazy, new_op, qlo, qhi);
		nodes[a].c = {x, y};
		nodes[a].val = merge(nodes[x].val, nodes[y].val);
		nodes[a].lazy = op_t{};

		return a;
	});

	for (int q = 1; q <= Q; q++) {
		int op; cin >> op;
		int t; cin >> t;
		if (op == 1) {
			int p; cin >> p; p--;
			assert(0 <= p && p < K);
			int l, r; cin >> l >> r; l--;
			int x; cin >> x;

			op_t cur_op;
			cur_op.add[p] = x;

			roots[q] = do_update(roots[t], 0, N, op_t{}, cur_op, l, r);

			/*
			for (int i = 0; i < N; i++) {
				cout << do_query(roots[q], 0, N, op_t{}, i, i+1) << " \n"[i+1==N];
			}
			*/
		} else if (op == 2) {
			int p; cin >> p; p--;
			assert(0 <= p && p < K);
			int l, r; cin >> l >> r; l--;
			int x; cin >> x;

			op_t cur_op;
			cur_op.zero = (1 << p);
			cur_op.add[p] = x;

			roots[q] = do_update(roots[t], 0, N, op_t{}, cur_op, l, r);

			/*
			for (int i = 0; i < N; i++) {
				cout << do_query(roots[q], 0, N, op_t{}, i, i+1) << " \n"[i+1==N];
			}
			*/
		} else if (op == 3) {
			int l, r; cin >> l >> r; l--;
			int ans = do_query(roots[t], 0, N, op_t{}, l, r);
			cout << ans << '\n';
		} else assert(false);
	}

	return 0;
}
