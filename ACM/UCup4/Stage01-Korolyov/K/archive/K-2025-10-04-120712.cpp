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

struct treap_node {
	std::array<treap_node*, 2> c{nullptr, nullptr};
	treap_node* p = nullptr;
	std::mt19937::result_type pri;
	int lazy = 0;
	int val = 0;
	void apply_lazy(int v) {
		lazy += v;
		val += lazy;
	}
	void downdate() {
		if (c[0]) c[0]->apply_lazy(lazy);
		if (c[1]) c[1]->apply_lazy(lazy);
		lazy = 0;
	}
	void downdate_all() {
		if (p) p->downdate_all();
		downdate();
	}
};

treap_node* merge(treap_node* a, treap_node* b) {
	if (!a) return b;
	if (!b) return a;
	treap_node* r;
	if (a->pri < b->pri) {
		r = a;
		r->downdate();
		r->c[1] = merge(a->c[1], b);
		r->c[1]->p = r;
	} else {
		r = b;
		r->downdate();
		r->c[0] = merge(a, b->c[0]);
		r->c[0]->p = r;
	}
	return r;
}

std::pair<treap_node*, treap_node*> split_ge(treap_node* r, int v) {
	if (!r) { return {nullptr, nullptr}; }
	r->downdate();
	treap_node *a, *b;
	if (r->val >= v) {
		b = r;
		std::tie(a, b->c[0]) = split_ge(r->c[0], v);
		if (b->c[0]) b->c[0]->p = b;
	} else {
		a = r;
		std::tie(a->c[1], b) = split_ge(r->c[1], v);
		if (a->c[1]) a->c[1]->p = a;
	}
	return {a, b};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q, D; cin >> N >> Q >> D;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<std::array<int, 2>> queries(Q);
	// inclusive
	for (auto& [l, r] : queries) { cin >> l >> r; l--, r--; }
	std::vector<int> ans(Q);
	std::vector<std::vector<int>> by_r(N);
	for (int q = 0; q < Q; q++) {
		by_r[queries[q][1]].push_back(q);
	}

	std::mt19937 mt(48);
	std::vector<treap_node> nodes(N);
	std::set<int> alive;
	treap_node* root = nullptr;
	for (int i = 0; i < N; i++) {
		{
			// insert i
			nodes[i].pri = mt();
			nodes[i].val = D;
			nodes[i].lazy = 0;
			root = merge(root, &nodes[i]);
			if (root) root->p = nullptr;
			alive.insert(i);
		}

		if (A[i]) {
			cerr << "do " << A[i] << '\n';
			auto [lo, mihi] = split_ge(root, A[i]-1);
			if (lo) lo->p = nullptr;
			if (mihi) mihi->p = nullptr;
			auto [mi, hi] = split_ge(mihi, 2 * A[i]);
			if (mi) mi->p = nullptr;
			if (hi) hi->p = nullptr;

			if (mi) {
				// merge them all into the last one
				std::y_combinator([&](auto&& self, treap_node* cur) -> void {
					if (!cur) return;
					self(cur->c[0]);
					int i = int(cur - nodes.data());
					cerr << "erasing " << i << '\n';
					alive.erase(i);
					self(cur->c[1]);
				})(mi);
				while (mi->c[1]) mi = mi->c[1];
				mi->c[0] = mi->c[1] = mi->p = nullptr;
				mi->lazy = 0;
				mi->val = A[i] - 1;
				cerr << "reinsert " << int(mi - nodes.data()) << '\n';
				alive.insert(int(mi - nodes.data()));
			}

			if (hi) hi->apply_lazy(-A[i]);

			mihi = merge(mi, hi);
			if (mihi) mihi->p = nullptr;
			root = merge(lo, mihi);
			if (root) root->p = nullptr;
		}

		for (int q : by_r[i]) {
			int l = queries[q][0];
			l = *alive.lower_bound(l);
			nodes[l].downdate_all();
			ans[q] = nodes[l].val;
		}
	}

	for (int q = 0; q < Q; q++) cout << ans[q] << '\n';

	return 0;
}
