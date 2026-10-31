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

static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
std::mt19937 mt(FIXED_RANDOM);
struct treap {
	std::array<treap*, 2> c{nullptr, nullptr};
	std::mt19937::result_type pri = mt();
	std::pair<int, int> val;
	int sz;
	int tot_cost;

	void update() {
		sz = 1 + (c[0] ? c[0]->sz : 0) + (c[1] ? c[1]->sz : 0);

		tot_cost = val.first + (c[0] ? c[0]->sz : 0);
		if (c[0]) tot_cost = std::max(tot_cost, c[0]->tot_cost);
		if (c[1]) tot_cost = std::max(tot_cost, (c[0] ? c[0]->sz : 0) + 1 + c[1]->tot_cost);
	}
};

treap* merge(treap* a, treap* b) {
	if (!a) return b;
	if (!b) return a;
	treap *r;
	if (a->pri < b->pri) {
		r = a;
		r->c[1] = merge(a->c[1], b);
	} else {
		r = b;
		r->c[0] = merge(a, b->c[0]);
	}
	r->update();
	return r;
}
// split at < mid / >= mid
std::pair<treap*, treap*> split(treap* r, std::pair<int, int> mid) {
	if (!r) return {nullptr, nullptr};
	treap *a, *b;
	if (r->val < mid) {
		a = r;
		std::tie(a->c[1], b) = split(r->c[1], mid);
	} else {
		b = r;
		std::tie(a, b->c[0]) = split(r->c[0], mid);
	}
	r->update();
	return {a, b};
}

treap* insert(treap* r, treap* n) {
	n->c = {nullptr, nullptr};
	n->update();
	auto [a, b] = split(r, n->val);
	return merge(merge(a, n), b);
}

treap* remove(treap* r, treap* n) {
	auto [a, mb] = split(r, n->val);
	auto [m, b] = split(mb, {n->val.first, n->val.second + 1});
	assert(m == n);
	assert(!m->c[0] && !m->c[1]);
	return merge(a, b);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	vector<int> par(N, -1);
	vector<int> dp_down(N);
	vector<int> dp_up(N);

	vector<treap> nodes(N);

	std::y_combinator([&](auto self, int cur, int prv) -> void {
		par[cur] = prv;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
		}
		if (prv == -1) return;
		treap* ROOT = nullptr;
		{
			nodes[cur].val = {0, cur};
			ROOT = insert(ROOT, &nodes[cur]);
		}
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			nodes[nxt].val = {dp_down[nxt], nxt};
			ROOT = insert(ROOT, &nodes[nxt]);
		}
		assert(ROOT);
		dp_down[cur] = ROOT->tot_cost;
	})(0, -1);

	std::y_combinator([&](auto self, int cur, int prv) -> void {
		treap* ROOT = nullptr;
		{
			nodes[cur].val = {0, cur};
			ROOT = insert(ROOT, &nodes[cur]);
		}
		for (int nxt : adj[cur]) {
			if (nxt == prv) {
				nodes[nxt].val = {dp_up[cur], nxt};
			} else {
				nodes[nxt].val = {dp_down[nxt], nxt};
			}
			ROOT = insert(ROOT, &nodes[nxt]);
		}
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			ROOT = remove(ROOT, &nodes[nxt]);
			assert(ROOT);
			dp_up[nxt] = ROOT->tot_cost;
			ROOT = insert(ROOT, &nodes[nxt]);
		}
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
		}
	})(0, -1);

	return 0;
}
