#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

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

	int N; cin >> N;
	std::vector<int> W(N);
	for (auto& w : W) cin >> w;

	std::vector<std::pair<int, int>> weights(N);
	for (int i = 0; i < N; i++) {
		weights[i] = {W[i], i};
	}
	std::sort(weights.begin(), weights.end());

	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	struct treap_node {
		std::array<int, 2> c = {-1, -1};
		std::mt19937::result_type pri;
		int k;
		int64_t own_left;
		int64_t own_right;
		int64_t best_left;
		int64_t best_right;
		int64_t lazy;
	};
	std::vector<treap_node> nodes(N);
	auto apply_lazy = [&](int i, int64_t v) -> void {
		if (i == -1) return;
		nodes[i].own_left += v;
		nodes[i].own_right += v;
		nodes[i].best_left += v;
		nodes[i].best_right += v;
		nodes[i].lazy += v;
	};
	auto update = [&](int i) -> void {
		assert(nodes[i].lazy == 0);
		nodes[i].best_left = nodes[i].own_left;
		nodes[i].best_right = nodes[i].own_right;
		for (int c : nodes[i].c) {
			if (c != -1) {
				setmax(nodes[i].best_left, nodes[c].best_left);
				setmax(nodes[i].best_right, nodes[c].best_right);
			}
		}
	};
	auto downdate = [&](int i) -> void {
		if (nodes[i].lazy) {
			for (int c : nodes[i].c) {
				if (c != -1) {
					apply_lazy(c, nodes[i].lazy);
				}
			}
			nodes[i].lazy = 0;
		}
	};
	auto split = std::y_combinator([&](auto self, int i, int k) -> std::pair<int, int> {
		if (i == -1) return {-1, -1};
		downdate(i);
		int l, r;
		if (k < nodes[i].k) {
			r = i;
			std::tie(l, nodes[r].c[0]) = self(nodes[i].c[0], k);
		} else if (k > nodes[i].k) {
			l = i;
			std::tie(nodes[l].c[1], r) = self(nodes[i].c[1], k);
		} else assert(false);
		update(i);
		return {l, r};
	});
	auto merge = std::y_combinator([&](auto self, int l, int r) -> int {
		if (l == -1) return r;
		if (r == -1) return l;
		int n;
		if (nodes[l].pri < nodes[r].pri) {
			n = l;
			downdate(n);
			nodes[n].c[1] = self(nodes[l].c[1], r);
		} else {
			n = r;
			downdate(n);
			nodes[n].c[0] = self(l, nodes[r].c[0]);
		}
		update(n);
	});
	auto treap_union = std::y_combinator([&](auto self, int a, int b) -> std::pair<int, int64_t> {
		int64_t best = 0;
		if (a == -1) return {b, best};
		if (b == -1) return {a, best};
		if (nodes[a].pri >= nodes[b].pri) std::swap(a, b);
		downdate(a);
		auto [l, r] = split(b, nodes[a].k);
		if (l != -1) {
			setmax(best, nodes[l].best_left + nodes[a].own_right);
			if (nodes[a].c[1] != -1) {
				setmax(best, nodes[l].best_left + nodes[nodes[a].c[1]].best_right);
			}
		}
		if (r != -1) {
			setmax(best, nodes[a].own_left + nodes[r].best_right);
			if (nodes[a].c[0] != -1) {
				setmax(best, nodes[nodes[a].c[0]].best_left + nodes[r].best_right);
			}
		}
		auto [nc0, b0] = self(nodes[a].c[0], l);
		setmax(best, b0);
		nodes[a].c[0] = nc0;
		auto [nc1, b1] = self(nodes[a].c[1], r);
		setmax(best, b1);
		nodes[a].c[1] = nc1;
		update(a);
		return {a, best};
	});
	auto dump_treap = std::y_combinator([&](auto self, int i, int64_t lz = 0) -> void {
		cerr << i << ' ' << nodes[i].own_left + lz << ' ' << nodes[i].own_right + lz << '\n';
	});

	for (int i = 0; i < N; i++) {
		nodes[weights[i].second].k = i;
	}
	std::mt19937 mt(48);
	for (int i = 0; i < N; i++) {
		nodes[i].pri = mt();
		nodes[i].own_left = W[i];
		nodes[i].own_right = 0;
		nodes[i].lazy = 0;
		update(i);
	}

	auto ans = std::y_combinator([&](auto self, int cur, int prv) -> std::pair<int64_t, int> {
		int64_t v = 0;
		int cur_tree = cur;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			auto [nv, nxt_tree] = self(nxt, cur);
			auto [new_tree, merge_v] = treap_union(cur_tree, nxt_tree);
			apply_lazy(new_tree, -merge_v);
			v += merge_v;
			cur_tree = new_tree;
		}
		cerr << "solve " << cur << '\n';
		cerr << "value " << v << '\n';
		dump_treap(cur_tree);
		cerr << '\n';
		return {v, cur_tree};
	})(0, -1).first;
	cout << ans << '\n';

	return 0;
}
